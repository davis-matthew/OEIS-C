/*
 * A224811  Number of subsets of {1,2,...,n-8} without differences equal to 2,
 *          4, 6 or 8.
 * https://oeis.org/A224811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 44, u32 n <= 83, u64 n <= 162,
 *            u128 n <= 320, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224811
 *  Number of subsets of {1,2,…,n-8} with no two elements differing by 2,4,6
 *  or 8.  The value a(n) is computed by a dynamic‑programming algorithm
 *  that keeps a mask of the last eight chosen elements.  The DP state
 *  transitions in O(1) per element, giving an overall O(n) algorithm
 *  suitable for queries in arbitrary order.
 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;     /* cache[n] = a(n)   */
static size_t cache_cap = 0;    /* allocated size   */
static idx_t cache_max = 0;     /* highest n with a(n) cached */

static seq_t dp_cur[256];       /* DP state for the last 8 elements */
static idx_t cur_m = 0;         /* number of processed elements (>=0) */
static bool initialized = false;

static void init_dp(void)
{
    for (int i = 0; i < 256; ++i)
        dp_cur[i] = 0;
    dp_cur[0] = 1;      /* start with an empty subset before any element */
    cur_m   = 0;        /* 0 elements processed correspond to n = 8   */
    initialized = true;

    /* a(8) is 1 */
    ensure_cache(8, &cache, &cache_cap);
    cache[8] = 1;
    cache_max = 8;
}

seq_t a(idx_t n)
{
    /* For 0 ≤ n ≤ 8 the subset is empty ⇒ only one subset. */
    if (n < 8)
        return (seq_t)1;

    ensure_cache(n, &cache, &cache_cap);

    if (!initialized)
        init_dp();

    if (n <= cache_max)
        return cache[n];

    /* Extend DP until we computed a(n). */
    while (cur_m < (n - 8)) {
        seq_t newdp[256] = {0};

        for (int mask = 0; mask < 256; ++mask) {
            seq_t val = dp_cur[mask];
            if (val == 0)
                continue;

            /* Option 0: skip the next element. */
            int newmask0 = (mask << 1) & 0xFF;
            newdp[newmask0] += val;

            /* Option 1: pick the next element, if all forbidden
             * differences are currently unchosen (bits 1,3,5,7). */
            if (!(mask & 0x02) && !(mask & 0x08) &&
                !(mask & 0x20) && !(mask & 0x80)) {
                int newmask1 = newmask0 | 1;
                newdp[newmask1] += val;
            }
        }

        /* Move to the next state. */
        for (int i = 0; i < 256; ++i)
            dp_cur[i] = newdp[i];
        ++cur_m;                     /* processed one more element */

        /* The current a(cur_m+8) equals the sum of all DP states. */
        seq_t total = 0;
        for (int i = 0; i < 256; ++i)
            total += dp_cur[i];
        idx_t idx = cur_m + 8;
        cache[idx] = total;
    }

    cache_max = n;
    return cache[n];
}
