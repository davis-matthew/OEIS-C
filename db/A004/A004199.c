/*
 * A004199  Table of [ x/y ], where (x,y) =
 *          (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),...
 * https://oeis.org/A004199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = floor(x/y), where (x, y) is the n-th pair in the diagonal
 *            enumeration of the 2D table entry(row, col) = floor(row/col)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>


seq_t a(idx_t n)
{
    // Find diagonal d such that (d-1)*d/2 >= n
    double d_double = (1.0 + sqrt(1.0 + 8.0 * (double)n)) / 2.0;
    idx_t d = (idx_t)ceil(d_double);
    
    // Verify in case of floating-point rounding
    while ((d - 1) * d / 2 < n) {
        d++;
    }
    
    // Position within diagonal (1-indexed)
    idx_t pos = n - (d - 2) * (d - 1) / 2;
    
    // Coordinates in the (x, y) enumeration
    idx_t x = pos;
    idx_t y = d - pos;
    
    // Return floor(x/y)
    return (seq_t)x / (seq_t)y;
}
