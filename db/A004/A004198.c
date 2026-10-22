/*
 * A004198  Table of x AND y, where (x,y) =
 *          (0,0),(0,1),(1,0),(0,2),(1,1),(2,0),...
 * https://oeis.org/A004198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, look
 * Formula:   a(n) = AND(i, j) where (i,j) is the position of n in the
 *            antidiagonal table
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
        
    // Find which diagonal and position within it
    uint64_t d = (uint64_t)floor((-1.0 + sqrt(1.0 + 8.0 * (double)n)) / 2.0);
    uint64_t k = n - d * (d + 1) / 2;
    
    // Return bitwise AND of coordinates (d-k, k)
    return (seq_t)((d - k) & k);
}
