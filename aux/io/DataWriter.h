#ifndef DATAWRITER
#define DATAWRITER
#include "ErrorHandling.h"
#include <iostream>
#include <vector>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkPointData.h>
#include <vtkPoints.h>

class DataWriter
{
	public:
	DataWriter(void){};
	void write(std::vector<double> colorsIn, std::vector<std::pair<double, double> > pointsIn, const char *filename);
};

void DataWriter::write(std::vector<double> colorsIn, std::vector<std::pair<double, double> > pointsIn, const char *filename)
{
        int i, i2=0;
	vtkDoubleArray *colorsOut = vtkDoubleArray::New();
	vtkDoubleArray *hashOut = vtkDoubleArray::New();
        vtkPolyDataWriter *writer = vtkPolyDataWriter::New();
        vtkPolyData *tris = vtkPolyData::New();
        vtkPoints *points = vtkPoints::New();
        vtkCellArray *polys = vtkCellArray::New();
        //vtkFloatArray *scalars = vtkFloatArray::New();
	vtkIdType vertices[3];
	double colorsToStore[3];
	double hashToStore = 3.33333333;//TODO = Encryption::GetHash();
	double ptLocs[3];

	const int colorsSize = colorsIn.size();
        const int ptsLen = pointsIn.size();

	try
	{
		if(ptsLen%3 != 0){throw myErr.SetErr(INVALIDNUMPTS);} //ensure there are enough points to build entirely whole triangles with the data
		if(colorsSize%3 != 0){throw myErr.SetErr(INVALIDNUMTRIS);}	
		if((double)colorsSize/3 != ptsLen){throw myErr.SetErr(UNEQUALLENS);}
		if(filename == nullptr || filename[0] == '\0'){throw myErr.SetErr(BADFILENAME);}
		
		hashOut->SetNumberOfComponents(1);
		hashOut->SetName("HASH");
		hashOut->InsertNextTuple(&hashToStore);

		colorsOut->SetNumberOfComponents(3);
		colorsOut->SetName("COLORS");
		for(i=0; i<colorsSize; i+=3)
		{
			colorsToStore[0] = colorsIn[i  ];
			colorsToStore[1] = colorsIn[i+1];
			colorsToStore[2] = colorsIn[i+2];
			colorsOut->InsertNextTuple(colorsToStore);
		}

		for(i=0; i<ptsLen; i+=3)
		{
			for(i2=0; i2<3; i2++)
			{
				vertices[i2] = i+i2;
				ptLocs[0] = pointsIn[i+i2].first;
				ptLocs[1] = pointsIn[i+i2].second;
				ptLocs[2] = 0;
				points->InsertPoint(i+i2, ptLocs);
			}
			polys->InsertNextCell(3, vertices);
	//		scalars->InsertTuple1(i,i);
		}
        	// We now assign the pieces to the vtkPolyData.
		tris->GetFieldData()->AddArray(colorsOut);
        	tris->SetPoints(points);
        	points->Delete();
        	tris->SetPolys(polys);
        	polys->Delete();
        //	tris->GetPointData()->SetScalars(scalars);
       // 	scalars->Delete();
	
	        writer->SetInputData(tris);
        	writer->SetFileName(filename);
        	writer->Write();
	}
	catch(invalidData e)
	{
		cerr << "AN ERROR OCCURED at " << __func__ << "!" << endl;
		cerr << "ERROR: " << e.errorMsg() << endl;
	}
}
#endif