/*
 * A195593  Number of ways to place 5n nonattacking kings on a vertical
 *          cylinder 10 X 2n.
 * https://oeis.org/A195593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -4*a(n-6) + 20*a(n-5) - 41*a(n-4) + 44*a(n-3) - 26*a(n-2)
 *            + 8*a(n-1) * Initial values (offset = 1): * a(1)=64, a(2)=732,
 *            a(3)=4392, a(4)=18890, a(5)=66532, a(6)=205628
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 17, u64 n <= 47, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined below the offset, harmless */

    if (n <= 6)
    {
        static const seq_t init[6] = {64, 732, 4392, 18890, 66532, 205628};
        return init[n - 1];
    }

    /* circular buffer for the last six terms */
    seq_t buf[6];
    buf[0] = 64;
    buf[1] = 732;
    buf[2] = 4392;
    buf[3] = 18890;
    buf[4] = 66532;
    buf[5] = 205628;

    idx_t i;
    for (i = 7; i <= n; ++i)
    {
        /* Compute positive and negative contributions separately
           to avoid underflow on unsigned arithmetic.                     */
        seq_t pos = 8 * buf[5] + 44 * buf[3] + 20 * buf[1];
        seq_t neg = 26 * buf[4] + 41 * buf[2] + 4 * buf[0];
        seq_t val = pos - neg;              /* pos >= neg always holds */

        /* shift buffer: drop the oldest and add the new value */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = val;
    }

    return buf[5];
}
