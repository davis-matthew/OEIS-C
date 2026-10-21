/*
 * A089110  Sign twisted convoluted convolved Fibonacci numbers H_5^(r).
 * https://oeis.org/A089110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-2) + 12*a(n-3) - 14*a(n-4) * + 12*a(n-5)
 *            - 8*a(n-6) + 4*a(n-7) - a(n-8) for n > 8 * Initial values:
 *            a(1)=5, a(2)=11, a(3)=17, a(4)=25, * a(5)=38, a(6)=54, a(7)=70,
 *            a(8)=89
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* pre‑computed first eight terms */
    static const seq_t init[8] = {5, 11, 17, 25, 38, 54, 70, 89};

    /* for n within first eight terms return directly */
    if (n <= 8ULL)
        return init[(size_t)n - 1u];

    /* keep the last eight values in a rotating buffer */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    seq_t next;
    for (idx_t i = 9; i <= n; ++i)
    {
        /* recurrence for a(i) */
        next = 4  * buf[7] - 8  * buf[6] + 12 * buf[5]
             - 14 * buf[4] + 12 * buf[3] - 8  * buf[2]
             + 4  * buf[1] -     buf[0];

        /* shift buffer: drop oldest, append newest */
        for (size_t k = 0; k < 7; ++k)
            buf[k] = buf[k + 1];
        buf[7] = next;
    }

    return buf[7];  /* a(n) */
}
