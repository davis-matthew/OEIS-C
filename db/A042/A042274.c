/*
 * A042274  Numerators of continued fraction convergents to sqrt(663).
 * https://oeis.org/A042274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 206*a(n-4) - a(n-8), a(0)=25, a(1)=26, a(2)=77, a(3)=103,
 *            a(4)=5227, a(5)=5330, a(6)=15887, a(7)=21217
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base values for 0 ≤ n < 8 */
    static const seq_t base[8] = {
        (seq_t)25,  (seq_t)26,  (seq_t)77,  (seq_t)103,
        (seq_t)5227,(seq_t)5330,(seq_t)15887,(seq_t)21217
    };

    if (n < 8)
        return base[n];

    /* Circular buffer holding the last 8 computed terms */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = base[i];

    seq_t next = 0;
    for (idx_t i = 8; i <= n; ++i)
    {
        /* a(i) = 206*a(i-4) - a(i-8) */
        next = (seq_t)206 * buf[4] - buf[0];

        /* Shift buffer left by one */
        for (idx_t k = 0; k < 7; ++k)
            buf[k] = buf[k + 1];
        buf[7] = next;
    }

    return next;
}
