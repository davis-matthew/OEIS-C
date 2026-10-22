/*
 * A003987  Table of n XOR m (or Nim-sum of n and m) read by antidiagonals
 *          with m>=0, n>=0.
 * https://oeis.org/A003987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, nice, look, hear
 * Formula:   a(n) = table of i XOR j read by antidiagonals, where i, j >= 0
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>


seq_t a(idx_t n) {
    /* Compute antidiagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    double d_float = (-1.0 + sqrt(1.0 + 8.0 * (double)n)) / 2.0;
    idx_t d = (idx_t)d_float;
    
    /* Adjust d if floating point rounding is inexact */
    while (d * (d + 1) / 2 > n) d--;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    
    /* Compute position within antidiagonal and table coordinates */
    idx_t p = n - d * (d + 1) / 2;
    seq_t row = (seq_t)(d - p);
    seq_t col = (seq_t)p;
    
    return row ^ col;
}
