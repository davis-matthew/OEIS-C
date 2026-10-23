/*
 * A038341  Fourth line of 5-wave sequence A038201.
 * https://oeis.org/A038341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A038341: a(n) satisfies a(0)=1, a(1)=2, a(2)=14, a(3)=29, a(4)=175,
   a(5)=365, a(6)=2163, a(7)=4516, a(8)=26703, a(9)=55759,
   and for n ≥ 10:
     a(n) = 15*a(n-2) - 35*a(n-4) + 28*a(n-6) - 9*a(n-8) + a(n-10). */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial terms a(0)...a(9) */
    static const seq_t init[10] = {
        1ULL, 2ULL, 14ULL, 29ULL, 175ULL,
        365ULL, 2163ULL, 4516ULL, 26703ULL, 55759ULL
    };

    if (n < 10)
        return init[(size_t)n];

    /* buffer holding the last 10 values: buf[0] = a(i-10), ..., buf[9] = a(i-1) */
    seq_t buf[10];
    for (int j = 0; j < 10; ++j)
        buf[j] = init[j];

    for (idx_t i = 10; i <= n; ++i) {
        /* compute a(i) from a(i-2), a(i-4), a(i-6), a(i-8), a(i-10) */
        seq_t val = 15 * buf[8]
                  - 35 * buf[6]
                  + 28 * buf[4]
                  - 9  * buf[2]
                  + buf[0];

        /* shift buffer left: drop the oldest, append new_val */
        for (int j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = val;
    }

    return buf[9];  /* a(n) after loop */
}
