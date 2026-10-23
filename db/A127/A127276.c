/*
 * A127276  Hankel transform of A127275.
 * https://oeis.org/A127276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^n - n*(n+1)*2^(n-2)
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 23, i64 n <= 53, i128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t 2^exp */

seq_t a(idx_t n)
{
    /* Fixed offset terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* factor = 4 - n*(n+1) */
    seq_t factor = (seq_t)4 - (seq_t)n * ((seq_t)n + (seq_t)1);

    /* two^(n-2) */
    seq_t pow = pow2(n - 2);

    /* a(n) = factor * 2^(n-2) */
    return factor * pow;
}
