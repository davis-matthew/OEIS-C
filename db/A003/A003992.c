/*
 * A003992  Square array read by upwards antidiagonals: T(n,k) = n^k for n >=
 *          0, k >= 0.
 * https://oeis.org/A003992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   a(n) = (d-p)^p where d = floor((-1+sqrt(1+8n))/2), p = n -
 *            d(d+1)/2
 * Range:     u8 n <= 39, u16 n <= 84, u32 n <= 200, u64 n <= 482,
 *            u128 n <= 1207, big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    // Find diagonal d using the quadratic formula
    idx_t d = (idx_t)floor((-1.0 + sqrt(1.0 + 8.0 * (double)n)) / 2.0);
    
    // Adjust for floating point rounding errors
    if ((d + 1) * (d + 2) / 2 <= n) d++;
    
    // Position within the diagonal
    idx_t start = (d * (d + 1)) / 2;
    idx_t p = n - start;
    
    // Compute (d-p)^p
    return a000248_pow((seq_t)(d - p), p);
}
