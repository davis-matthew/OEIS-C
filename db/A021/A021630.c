/*
 * A021630  Decimal expansion of 1/626.
 * https://oeis.org/A021630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * frac(10^n / 626))
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th decimal digit of 1/626 (0-indexed). */
/* Implementation uses simple long division. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t i;
    for (i = 0; i <= n; ++i)
    {
        r = r * (seq_t)10;
        seq_t d = r / (seq_t)626;
        r = r % (seq_t)626;
        if (i == n)
            return d;
    }
    return 0; /* unreachable */
}
