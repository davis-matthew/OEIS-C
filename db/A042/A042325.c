/*
 * A042325  Denominators of continued fraction convergents to sqrt(689).
 * https://oeis.org/A042325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 210*a(n-2) - a(n-4) for n>3; Initial values: a(0)=1,
 *            a(1)=4, a(2)=209, a(3)=840
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)209;
    if (n == 3) return (seq_t)840;

    /* Keep only the last four computed terms in a circular buffer */
    seq_t buf[4] = { (seq_t)1, (seq_t)4, (seq_t)209, (seq_t)840 };

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (210 * buf[(i + 2) & 3]) - buf[i & 3];
        buf[i & 3] = next;
    }

    return buf[n & 3];
}
