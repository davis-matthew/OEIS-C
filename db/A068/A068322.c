/*
 * A068322  Number of arithmetic progressions of positive odd integers,
 *          strictly increasing with sum n.
 * https://oeis.org/A068322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 791, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of arithmetic progressions of positive odd integers,
   strictly increasing, with sum n. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The single‑term progression is valid only if n itself is odd. */
    uint64_t cnt = (n & 1ULL) ? 1ULL : 0ULL;

    /* Progressions of length k >= 2.  For each k we solve
       k * a1 + d * k*(k-1)/2 = n   with  d even, a1 odd, a1 >= 1. */
    for (idx_t k = 2; k <= n; ++k)
    {
        uint64_t m = (uint64_t)k * (k - 1) / 2;   /* k*(k-1)/2 */
        if (m > (uint64_t)n) break;               /* no larger k can work */

        /* The largest even difference that can still give a1 >= 1: */
        uint64_t dmax = (uint64_t)(n - k) / m;
        if (dmax < 2) continue;                 /* need at least d = 2 */

        for (uint64_t d = 2; d <= dmax; d += 2)
        {
            uint64_t numer = (uint64_t)n - d * m;
            if (numer % k != 0) continue;       /* a1 must be integer */

            uint64_t a1 = numer / k;
            if (a1 < 1) continue;              /* safety */
            if ((a1 & 1ULL) == 0ULL) continue; /* a1 must be odd */

            ++cnt;
        }
    }

    return (seq_t)cnt;
}
