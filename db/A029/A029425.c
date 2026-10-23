/*
 * A029425  Expansion of 1/((1-x^6)(1-x^8)(1-x^9)(1-x^12)).
 * https://oeis.org/A029425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029425:  Expansion of 1/((1-x^6)(1-x^8)(1-x^9)(1-x^12))
 * Recurrence (valid for n >= 35):
 *   a(n) =  a(n-6) + a(n-8) + a(n-9) + a(n-12)
 *         - a(n-14)- a(n-15)- a(n-17)- a(n-18)- a(n-20)
 *         - a(n-21)+ a(n-23)+ a(n-26)+ a(n-27)+ a(n-29)
 *         - a(n-35)
 * Base values for 0 <= n < 35 are obtained by standard partition DP.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_last = 0;

/* Offsets and corresponding signs for the recurrence. */
static const idx_t offsets[] = {6, 8, 9,12,14,15,17,18,20,21,23,26,27,29,35};
static const int  signs  [] = { 1, 1, 1, 1,-1,-1,-1,-1,-1,-1, 1, 1, 1, 1,-1};
static const size_t rec_len = sizeof(offsets)/sizeof(offsets[0]);

/* Compute the base values up to 34 using the direct DP method. */
static void
init_base(void)
{
    const idx_t base_limit = 34;
    /* Ensure cache can hold up to base_limit. */
    if (cache_size <= base_limit) {
        seq_t *tmp = (seq_t *)realloc(cache, (base_limit+1) * sizeof(seq_t));
        if (!tmp) return;
        cache = tmp;
        cache_size = base_limit + 1;
    }
    /* Initialize all entries to 0. */
    memset(cache, 0, cache_size * sizeof(seq_t));
    cache[0] = 1;
    const idx_t parts[] = {6,8,9,12};
    for (size_t p = 0; p < sizeof(parts)/sizeof(parts[0]); ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= base_limit; ++i) {
            cache[i] += cache[i - part];
        }
    }
    cache_last = base_limit;
}

/* Ensure the cache is large enough up to n. */
static void
extend_cache(idx_t n)
{
    if (n < cache_size) return;
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return;
    cache = tmp;
    cache_size = n + 1;
    /* Newly allocated entries are already zeroed by realloc. */
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Initialize if first call. */
    if (!cache) {
        init_base();
    }
    /* Extend cache if necessary. */
    if (n >= cache_size) {
        extend_cache(n);
    }

    /* Compute values incrementally beyond the last computed index. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* For i <= 34 use direct DP (already computed). */
        if (i <= 34) {
            /* Nothing to do; values already set. */
            continue;
        }
        seq_t pos = 0;
        seq_t neg = 0;
        for (size_t r = 0; r < rec_len; ++r) {
            idx_t off = offsets[r];
            if (i >= off) {
                if (signs[r] > 0)
                    pos += cache[i - off];
                else
                    neg += cache[i - off];
            }
        }
        cache[i] = pos - neg; /* pos >= neg by definition. */
    }

    /* Update last computed index. */
    if (n > cache_last) cache_last = n;
    return cache[n];
}
