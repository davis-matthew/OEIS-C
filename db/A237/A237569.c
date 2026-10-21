/*
 * A237569  Number of (n+1)X(1+1) 0..3 arrays with the upper median minus the
 *          lower median of every 2X2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A237569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence from OEIS A237569:
   a(n) = 23*a(n-2) + 1473*a(n-4) + 15833*a(n-6) + 60176*a(n-8)
          + 65344*a(n-10) - 70656*a(n-12) - 139264*a(n-14)   for n > 15
   Initial values:
   a(1)=256,  a(2)=1876,  a(3)=13924,  a(4)=103264,  a(5)=769680,
   a(6)=5705156, a(7)=42501300, a(8)=315065200, a(9)=2347267424,
   a(10)=17400203636, a(11)=129631996580, a(12)=960959725312,
   a(13)=7159191300272, a(14)=53070925060004, a(15)=395380448791316 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1‑based array of initial terms (index 0 unused) */
    static const seq_t init[16] = {
        0, 256ull, 1876ull, 13924ull, 103264ull, 769680ull,
        5705156ull, 42501300ull, 315065200ull, 2347267424ull,
        17400203636ull, 129631996580ull, 960959725312ull,
        7159191300272ull, 53070925060004ull, 395380448791316ull
    };

    if (n <= 15)
        return init[(size_t)n];

    /* ring buffer: buf[0]=a(i‑1), buf[1]=a(i‑2), …, buf[14]=a(i‑15)  */
    seq_t buf[15];
    for (int k = 0; k < 15; ++k)
        buf[k] = init[15 - k];   /* buf[0]=a(15), buf[14]=a(1) */

    seq_t val = 0;
    for (idx_t i = 16; i <= n; ++i)
    {
        val = 23 * buf[1] + 1473 * buf[3] + 15833 * buf[5]
              + 60176 * buf[7] + 65344 * buf[9]
              - 70656 * buf[11] - 139264 * buf[13];

        /* shift buffer: new buf[0] becomes the current value */
        for (int j = 14; j > 0; --j)
            buf[j] = buf[j - 1];
        buf[0] = val;
    }
    return val;
}
