/*
 * A041750  Numerators of continued fraction convergents to sqrt(395).
 * https://oeis.org/A041750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 318*a(n-4) - a(n-8) Initial values: a(0)=19, a(1)=20,
 *            a(2)=139, a(3)=159, a(4)=6181, a(5)=6340, a(6)=44221, a(7)=50561
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    const seq_t init[8] = {
        (seq_t)19, (seq_t)20, (seq_t)139, (seq_t)159,
        (seq_t)6181, (seq_t)6340, (seq_t)44221, (seq_t)50561
    };

    if (n < 8)
        return init[(size_t)n];

    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t prod = (seq_t)318 * buf[(i - 4) & 7];
        buf[i & 7] = prod - buf[(i - 8) & 7];
    }

    return buf[n & 7];
}
