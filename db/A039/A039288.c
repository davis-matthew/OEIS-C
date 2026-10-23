/*
 * A039288  Numbers whose base-5 representation has the same nonzero number of
 *          1's and 3's.
 * https://oeis.org/A039288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: numbers whose base-5 representation has the same nonzero number of 1's and 3's. */
/* Computed by scanning integers and selecting those with equal counts of 1 and 3. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cached sequence values         */
    static size_t cache_len = 0;       /* number of values cached        */
    static size_t cache_cap = 0;       /* current allocation capacity    */
    static idx_t next_candidate = 1;   /* next integer to test           */

    if (n == 0) return 0;              /* offset is 1; behavior for 0 is arbitrary */

    while (cache_len < n)              /* generate terms until we have n of them */
    {
        idx_t cand = next_candidate;   /* start from the last tested number   */
        for (;; ++cand)                /* search for the next qualifying number */
        {
            idx_t tmp  = cand;
            idx_t cnt1 = 0, cnt3 = 0;
            while (tmp > 0)            /* count 1's and 3's in base‑5 digits    */
            {
                int d = tmp % 5;
                if (d == 1) ++cnt1;
                else if (d == 3) ++cnt3;
                tmp /= 5;
            }
            if (cnt1 > 0 && cnt1 == cnt3)     /* equal non‑zero counts of 1 and 3 */
            {
                /* store the qualifying number, expanding the cache if needed */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    seq_t *new_cache = (seq_t *)realloc(cache,
                                                         new_cap * sizeof(seq_t));
                    if (!new_cache)          /* allocation failure: abort */
                    {
                        free(cache);
                        cache = NULL;
                        cache_len = 0;
                        cache_cap = 0;
                        return (seq_t)0;
                    }
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cand;
                break;                 /* found one candidate, stop searching */
            }
        }
        next_candidate = cand + 1;       /* remember where we left off          */
    }

    return cache[n - 1];                /* 1‑based indexing                     */
}
