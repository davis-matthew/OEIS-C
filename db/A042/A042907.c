/*
 * A042907  Denominators of continued fraction convergents to sqrt(985).
 * https://oeis.org/A042907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 816*a(n-5) + a(n-10) for n > 9
 * Initial values:
 * a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=13,
 * a(5)=811, a(6)=1635, a(7)=2446, a(8)=4081, a(9)=10608
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* fixed initial terms */
    const seq_t init[10] = {
        (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)13,
        (seq_t)811, (seq_t)1635, (seq_t)2446, (seq_t)4081, (seq_t)10608
    };

    /* for very small n return the known value */
    if (n < 10)
        return init[(size_t)n];

    idx_t i;
    seq_t buf[10];
    for (i = 0; i < 10; ++i)
        buf[i] = init[i];

    /* compute up to the requested n */
    for (i = 10; i <= n; ++i)
    {
        size_t pos     = (size_t)(i % 10);
        size_t pos5    = (size_t)((i - 5) % 10);
        size_t pos10   = (size_t)((i - 10) % 10);
        buf[pos] = buf[pos5] * (seq_t)816 + buf[pos10];
    }

    return buf[(size_t)(n % 10)];
}
