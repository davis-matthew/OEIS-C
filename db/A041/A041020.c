/*
 * A041020  Numerators of continued fraction convergents to sqrt(14).
 * https://oeis.org/A041020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 30*a(n-4) - a(n-8) with initial values: a(0)=3, a(1)=4,
 *            a(2)=11, a(3)=15, a(4)=101, a(5)=116, a(6)=333, a(7)=449
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* buffer for the last 8 terms */
    seq_t buf[8] = { (seq_t)3, (seq_t)4, (seq_t)11, (seq_t)15,
                     (seq_t)101, (seq_t)116, (seq_t)333, (seq_t)449 };

    if (n < (idx_t)8)
        return buf[(size_t)n];

    for (idx_t i = (idx_t)8; i <= n; ++i)
    {
        /* compute a(i) from a(i-4) and a(i-8) */
        seq_t new_val = (seq_t)30 * buf[(i - 4u) & 7u]
                        - buf[(i - 8u) & 7u];
        buf[i & 7u] = new_val;
    }

    return buf[n & 7u];
}
