/*
 * A031530  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 32.
 * https://oeis.org/A031530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A031530 – brute‑force search for integers k such that (i) √k has
 *            an even continued‑fraction period, (ii) after deleting the last
 *            term of the period, the central term is 32. The function builds
 *            the sequence incrementally and caches found terms. The cache
 *            size is fixed at 20000 which covers the req...
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--- includes -----------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*--- integer square root (floor) for 64‑bit values ---------------------------*/
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/*--- test for perfect square -------------------------------------------------*/
static inline bool is_square_u64(idx_t n)
{
    if (n < 2) return false;
    idx_t r = isqrt_u64(n);
    return (r * r == n);
}

/*--- cache and candidate marker ----------------------------------------------*/
static seq_t  cache[20000];   /* enough for n ≤ 10000 */
static size_t cache_len  = 0;
static idx_t  next_cand   = 1; /* first try is 2 */

/*--- period storage (static to avoid dynamic allocation) ---------------------*/
static idx_t period[65536];   /* should be enough for all practical k */

/*--- main sequence function --------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but safe */

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        /* find the next integer k with the required property */
        for (;;) {
            ++next_cand;                     /* candidate k */
            if (is_square_u64(next_cand))    /* skip perfect squares */
                continue;

            /* continued‑fraction expansion of √next_cand */
            idx_t k   = next_cand;
            idx_t a0  = isqrt_u64(k);
            idx_t m   = 0;
            idx_t d   = 1;
            idx_t a   = a0;                  /* first term after a0 */

            size_t i  = 0;                   /* count of period terms */

            for (;;) {
                /* recurrence: m1 = d*a - m */
                m = (idx_t)((unsigned __int128)d * a - m);
                /* d1 = (k - m*m) / d */
                d = (idx_t)((unsigned __int128)(k - (unsigned __int128)m * m) / d);
                /* a1 = (a0 + m) / d */
                a = (idx_t)((a0 + m) / d);

                period[i++] = a;           /* store term */

                if (a == 2 * a0)          /* end of period */
                    break;
            }

            /* test the required conditions */
            if ((i & 1) == 0 && period[i / 2 - 1] == 32) {
                /* store the found k */
                cache[cache_len++] = (seq_t)k;
                break;                    /* move on to next term needed */
            }
            /* otherwise continue searching */
        }
    }

    return cache[n - 1];
}
