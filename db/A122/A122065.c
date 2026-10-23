/*
 * A122065  Take sum of squares of digits of previous term; start with 74169.
 * https://oeis.org/A122065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of squares of decimal digits of a(n‑1), with a(1) =
 *            74169
 * Range:     u8 none, u16 none, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)74169;

    seq_t val = (seq_t)74169;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = 0;
        seq_t t   = val;
        while (t > 0) {
            unsigned d = (unsigned)(t % 10);
            sum += (seq_t)(d * d);
            t   /= 10;
        }
        val = sum;
    }
    return val;
}
