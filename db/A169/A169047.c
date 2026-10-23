/*
 * A169047  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169047
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

/* Sequence A169047: Number of reduced words of length n in a Coxeter group
 * with 34 generators and relations (S_i)^2 = (S_i S_j)^24 = I.
 *
 * Recurrence:
 *   a(0) = 1
 *   a(n) = 2 + 32 * Σ_{i=0}^{n-1} a(i)          for 1 ≤ n ≤ 23
 *   a(n) = 32 * Σ_{k=1}^{23} a(n-k) - 528 * a(n-24)  for n ≥ 24
 *
 * The first 24 terms are generated from the rational generating function
 * P(t)/Q(t) where Q(t) = 1 - 32t - 32t^2 - ... - 32t^23 + 528t^24.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of cached terms (next free index) */
static size_t cache_cap  = 0;   /* allocated capacity of cache */

static void ensure_cache(idx_t n)
{
    /* Ensure cache has space for indices 0 .. n */
    if ((idx_t)(cache_size) > n) return;
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i <= 23) {                /* first 24 terms */
            seq_t sum = 0;
            for (size_t j = 0; j < i; ++j) sum += cache[j];
            cache[i] = (i == 0 ? 1 : 2) + (32 * sum);
        } else {                      /* recurrence for n >= 24 */
            seq_t sum = 0;
            for (size_t k = 1; k <= 23; ++k) sum += cache[i - k];
            seq_t res = 32 * sum;
            res -= 528 * cache[i - 24];
            cache[i] = res;
        }
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
