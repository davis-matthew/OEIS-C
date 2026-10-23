/*
 * A042414  Numerators of continued fraction convergents to sqrt(735).
 * https://oeis.org/A042414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 488*a(n-2) - a(n-4) with a(0)=27, a(1)=244, a(2)=13203,
 *            a(3)=119071
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)27;
    if (n == 1) return (seq_t)244;
    if (n == 2) return (seq_t)13203;
    if (n == 3) return (seq_t)119071;

    /* ring buffer of size 4 holding a(i) for i modulo 4 */
    static seq_t buf[4];
    buf[0] = (seq_t)27;
    buf[1] = (seq_t)244;
    buf[2] = (seq_t)13203;
    buf[3] = (seq_t)119071;

    for (idx_t i = 4; i <= n; ++i)
    {
        idx_t idx2 = (i - 2) & 3u;
        idx_t idx4 = (i - 4) & 3u;
        buf[i & 3u] = (488 * buf[idx2]) - buf[idx4];
    }
    return buf[n & 3u];
}
