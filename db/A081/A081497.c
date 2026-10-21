/*
 * A081497  Duplicate of A028270.
 * https://oeis.org/A081497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = binom(2n, n) + binom(2n-2, n-1) (with binom(m,k) =
 *            C(m,k))
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence is A028270 in the OEIS. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)       /* a(0) = 1 by definition */
        return (seq_t)1;

    idx_t two_n  = n * 2;                /* 2·n  */
    seq_t term1 = binom(two_n, n);       /* C(2n, n)   */
    seq_t term2 = binom(two_n - 2, n - 1);/* C(2n-2, n-1) */
    return term1 + term2;
}
