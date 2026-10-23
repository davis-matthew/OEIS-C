/*
 * A170197  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for 1 ≤ n ≤ 38: a(n) = 2 + 42 * Σ_{k=0}^{n‑1} a(k);
 *            for n ≥ 39: a(n) = 42 * Σ_{k=n‑38}^{n‑1} a(k) - 903 * a(n‑39) +
 *            (n==39?1:0) (The extra '+1' occurs only for n = 39 because the
 *            numerator of the generating function contributes a constant term
 *            for that index.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;     /* sequence values */
static size_t cache_cap = 0;    /* capacity of cache memory */
static size_t cache_len = 0;    /* number of values already stored */

/* enlarge cache so that it can hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* lazy initialisation: a(0)=1 */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache if necessary */
    grow_cache((size_t)n + 1);

    /* compute terms that have not been cached yet */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* sum of the required previous terms */
        seq_t sum_last = 0;
        idx_t start = (i >= 38) ? (i - 38) : 0;
        for (idx_t j = start; j < i; ++j)
            sum_last += cache[j];

        /* start building the value */
        seq_t val = 42 * sum_last;

        /* constant term from the numerator of the G.F. */
        if (i == 0)
            val += (seq_t)1;
        else if (i <= 38)
            val += (seq_t)2;
        else if (i == 39)
            val += (seq_t)1;

        /* subtract the –903 * a(i‑39) part of the recurrence for i ≥ 39 */
        if (i >= 39)
            val -= (seq_t)903 * cache[i - 39];

        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
