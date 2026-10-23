/*
 * A164668  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A164668: Number of reduced words of length n in the Coxeter
    group on 32 generators with relations (S_i)^2 = (S_i S_j)^7 = I.
    Generating function:
        (t^7 + 2*t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1) /
        (465*t^7 - 30*t^6 - 30*t^5 - 30*t^4 - 30*t^3 - 30*t^2 - 30*t + 1)
    Recurrence (for n ≥ 7):
        a(n) = 30*a(n-1) + 30*a(n-2) + 30*a(n-3) + 30*a(n-4) +
               30*a(n-5) + 30*a(n-6) - 465*a(n-7) + δ(n,7)
    where δ(n,7) = 1 if n = 7, otherwise 0.
    Base values (n = 0 … 6) are:
        1, 32, 992, 30752, 953312, 29552672, 916132832
    The function memoises results so that calls may be in any order.   */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;    /* memoised terms a(0)…a(current-1)   */
static size_t cache_count = 0; /* number of terms stored (size ≥ cache_count) */

/* Ensure cache has room for indices 0 … n (inclusive). */
static void
grow_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_count >= need) return;

    size_t new_cap = cache_count == 0 ? 8 : cache_count;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_count * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;

    /* initialise base values if they are not yet stored */
    if (cache_count < 7) {
        static const seq_t base[7] = {
            (seq_t)1,
            (seq_t)32,
            (seq_t)992,
            (seq_t)30752,
            (seq_t)953312,
            (seq_t)29552672,
            (seq_t)916132832
        };
        for (idx_t i = cache_count; i < 7 && i < need; ++i)
            cache[i] = base[i];
    }

    /* compute missing terms */
    for (idx_t i = cache_count; i < need; ++i) {
        if (i < 7) continue;
        /* sum of the previous six terms */
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] +
                    cache[i-4] + cache[i-5] + cache[i-6];

        /* the recurrence part */
        seq_t term = sum * (seq_t)30 - cache[i-7] * (seq_t)465;

        /* extra contribution from the numerator for n = 7 only */
        if (i == 7u) term += (seq_t)1;

        cache[i] = term;
    }

    cache_count = need;
}

seq_t
a(idx_t n)
{
    grow_cache(n);
    return cache[(size_t)n];
}
