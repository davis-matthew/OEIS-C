/*
 * A126411  Number of base 24 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑24 n‑digit strings such that every pair of
 *            adjacent digits differs by at most 2. The sequence is defined by
 *            the recurrence a(0) = 1 a(1) = 24 a(n) = Σ_{d=0}^{23} a_{n-1}(d)
 *            where a_{n}(d) = number of such strings of length n ending with
 *            digit d and a_{k+1}(d) = Σ_{e: |e-d|≤2...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

static seq_t *sum_cache = NULL;        /* cache for a(n) */
static size_t cache_max = 0;           /* largest n cached */
static seq_t *dp_cur = NULL;           /* DP for current length */
static seq_t *dp_next = NULL;          /* DP for next length */
static int dp_ready = 0;               /* 0=not initialised, 1=initialised */

/* Expand the cache and DP up to the required index.
   This routine is idempotent for indices that have already been cached. */
static void ensure_cache(idx_t up_to)
{
    /* If the requested index is already cached, nothing to do */
    if (up_to <= cache_max) return;

    /* Grow the sum cache: allocate a new array large enough */
    seq_t *new_cache = seq_alloc_array(up_to + 1);
    if (!new_cache) exit(1);          /* allocation failure */

    /* Copy previously cached values */
    if (sum_cache) {
        for (size_t i = 0; i <= cache_max; ++i)
            new_cache[i] = sum_cache[i];
    }

    /* Re‑assign the cache pointer */
    sum_cache = new_cache;

    /* Initialise DP structures if this is the first call */
    if (!dp_ready) {
        dp_cur  = seq_alloc_array(24);
        dp_next = seq_alloc_array(24);
        if (!dp_cur || !dp_next) exit(1);
        for (int d = 0; d < 24; ++d) dp_cur[d] = 1;   /* length 1 strings */

        sum_cache[0] = 1;   /* a(0) */
        sum_cache[1] = 24;  /* a(1) */
        cache_max = 1;
        dp_ready = 1;
    }

    /* Build successive DP rows up to `up_to` */
    for (idx_t len = cache_max + 1; len <= up_to; ++len) {
        /* Compute dp_next[d] = Σ_{e: |e-d|≤2} dp_cur[e] */
        for (int d = 0; d < 24; ++d) {
            seq_t s = 0;
            for (int offs = -2; offs <= 2; ++offs) {
                int e = d + offs;
                if (e >= 0 && e < 24) s += dp_cur[e];
            }
            dp_next[d] = s;
        }

        /* Sum all endings to get a(len) */
        seq_t total = 0;
        for (int d = 0; d < 24; ++d) total += dp_next[d];
        sum_cache[len] = total;

        /* Prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    cache_max = up_to;
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 by definition */
    ensure_cache(n);
    return sum_cache[n];
}
