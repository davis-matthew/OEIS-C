/*
 * A089043  a(n) = n!^2 + (-1)^n.
 * https://oeis.org/A089043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n!)^2 + (-1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t fact_upto(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t f = fact_upto(n);
    /* square it */
    seq_t sq = f * f;
    /* add or subtract 1 depending on parity of n */
    if ((n & 1) == 0)
        return sq + 1;
    else
        return sq - 1;
}
