/*
 * A081907  Fifth binomial transform of binomial(n+2, 2).
 * https://oeis.org/A081907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n * (n^2 + 23*n + 72) / 72
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)61;

    /* For n >= 3, 6^n is divisible by 72.
       Compute 6^(n-3) first, then multiply by 3 to obtain 6^n / 72.  */
    seq_t term = seq_pow((seq_t)6, n - 3);  /* 6^(n-3) */
    term *= (seq_t)3;                      /* /72 factor */

    seq_t quad = (seq_t)n * (seq_t)n
               + (seq_t)23 * (seq_t)n
               + (seq_t)72;

    return term * quad;
}
