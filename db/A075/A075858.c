/*
 * A075858  n followed by n 1's.
 * https://oeis.org/A075858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = n*10^n + (10^n - 1)/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   pow10_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_simple.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used (offset is 1) */
    seq_t p = pow10_simple(n);             /* p = 10^n              */
    seq_t res = ((seq_t)n) * p;           /* n * 10^n              */
    seq_t repunit = (p - (seq_t)1) / (seq_t)9; /* (10^n-1)/9         */
    return res + repunit;
}
