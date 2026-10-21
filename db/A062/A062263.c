/*
 * A062263  Sixth (unsigned) column of triangle A062140 (generalized a=4
 *          Laguerre).
 * https://oeis.org/A062263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062263: a(n) = (n+5)! * binomial(n+9, 9) / 5!
 *          = (product_{i=6}^{n+5} i) * binomial(n+9, 9)
 * This uses only integer arithmetic and works for any seq_t that
 * supports +, -, *, /.
 */
#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* compute (n+5)! / 5! = product_{i=6}^{n+5} i */
    seq_t fact_part = 1;
    /* the loop is safe even when n+5 < 6 (i.e., n==0) */
    for (idx_t i = 6; i <= n + 5; ++i)
    {
        fact_part *= (seq_t)i;
    }

    /* compute binomial(n+9, 9) multiplicatively */
    seq_t binom = 1;
    idx_t N = n + 9;
    for (idx_t k = 1; k <= 9; ++k)
    {
        binom *= (seq_t)(N - (k - 1));
        binom /= (seq_t)k;
    }

    /* combine parts */
    return fact_part * binom;
}
