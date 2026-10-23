/*
 * A122182  Least positive number k such that 2^k mod k = 2n, or 0 if no such
 *          k exists.
 * https://oeis.org/A122182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Least positive k such that 2^k mod k = 2n, or 0 if no such k
 *            exists
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/powmod64.h"    /* powmod64(uint64_t base, uint64_t exp, uint64_t mod) */

seq_t a(idx_t n)
{
    /* cache for the first MAX_N+1 terms (0 … MAX_N) */
    enum { MAX_N = 10000 };
    static seq_t cache[MAX_N + 1];
    static bool init = false;

    if (!init) {
        /* initialise cache with 0 (unreached) */
        for (idx_t i = 0; i <= MAX_N; ++i) {
            cache[i] = 0;
        }

        /* search for k in increasing order until all needed n are found
           or until a generous limit.      */
        const idx_t LIMIT_K = 10000000;          /* 10 million.   */
        idx_t remaining = MAX_N + 1;             /* how many n still missing  */

        for (idx_t k = 1; k <= LIMIT_K && remaining > 0; ++k) {
            /* 2^k mod k, with k small enough for 64‑bit arithmetic. */
            uint64_t r64 = powmod64(2ULL, k, k);   /* r < k <= LIMIT_K */
            if ((r64 & 1ULL) == 0) {               /* r must be even   */
                idx_t n_calc = (idx_t)(r64 >> 1);   /* n = r/2         */
                if (n_calc <= MAX_N && cache[n_calc] == 0) {
                    cache[n_calc] = (seq_t)k;
                    --remaining;
                }
            }
        }
        /* if some n are still unfilled, leave them as 0 (no solution). */
        init = true;
    }

    if (n <= MAX_N) {
        return cache[(idx_t)n];
    } else {
        return 0;   /* beyond computed range – defined to return 0   */
    }
}
