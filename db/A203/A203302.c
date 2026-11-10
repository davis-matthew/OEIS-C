/*
 * A203302  Cumulative sums of A201206.
 * https://oeis.org/A203302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1/2)^(3/2)) = floor(sqrt((2n+1)^3/8)), the
 *            partial sums of A201206 telescoped
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A203302_isqrt(seq_t x)
{
    seq_t res = 0;
    seq_t bit = 1;
    while (bit <= x / 4)
        bit <<= 2;
    while (bit != 0)
    {
        if (x >= res + bit)
        {
            x -= res + bit;
            res = (res >> 1) + bit;
        }
        else
            res >>= 1;
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 2 + 1;
    seq_t v = t * t * t / 8;
    return A203302_isqrt(v);
}
