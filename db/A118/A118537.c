/*
 * A118537  Number of functions f: {1, 2, ..., n} --> {1, 2, ..., n} such that
 *          f(1) != f(2), f(2) != f(3), ..., f(n-1) != f(n), f(n) != f(1).
 * https://oeis.org/A118537
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)^n + (-1)^n*(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t nm1 = (seq_t)n - 1;           /* (n-1) as seq_t */
    seq_t p = seq_pow(nm1, n);           /* (n-1)^n  */
    if ((n & 1u) == 0)                   /* n even  */
        return p + nm1;                  /* + (n-1) */
    else                                 /* n odd   */
        return p - nm1;                  /* - (n-1) */
}
