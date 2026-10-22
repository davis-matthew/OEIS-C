/*
 * A003985  Triangle with subscripts (1,1),(2,1),(1,2),(3,1),(2,2),(1,3), etc.
 *          in which entry (i,j) is i AND j.
 * https://oeis.org/A003985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = i & j, where (i,j) is found by reading a triangle in
 *            anti-diagonal order
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    // Find which diagonal n belongs to using the quadratic formula
    idx_t d = (a000037_isqrt(1 + 8 * n) - 1) / 2;
    
    // Adjust for integer rounding
    while (d * (d + 1) / 2 < n) {
        d++;
    }
    
    // Position within the diagonal
    idx_t pos = n - (d - 1) * d / 2;
    
    // Coordinates in the triangle
    idx_t i = d + 1 - pos;
    idx_t j = pos;
    
    return (seq_t)i & (seq_t)j;
}
