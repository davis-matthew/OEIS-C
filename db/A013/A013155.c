/*
 * A013155  Expansion of e.g.f. exp(arctanh(x)+log(x+1)).
 * https://oeis.org/A013155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + ((2-n)^2-1)*a(n-2) Initial values: a(0)=1,
 *            a(1)=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated capacity (number of slots) */
static idx_t cache_len = 0;   /* number of values already stored (next free index) */

/* Grow the cache if needed and compute any missing values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)     /* already computed */
        return;

    /* Grow the cache when required */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;

        /* First allocation: initialise a(0) and a(1) */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            if (new_cap > 1) cache[1] = (seq_t)2;
            cache_len = (new_cap > 1 ? 2 : 1);
        }
    }

    /* Compute missing entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        if (i == 1) {
            cache[1] = (seq_t)2;
            continue;
        }
        if (i == 2) {
            seq_t temp = (seq_t)2 * cache[i-1];
            cache[i] = temp - cache[i-2];          /* coefficient = -1 for n=2 */
        } else {
            /* coefficient = (i-2)^2 - 1, which is non‑negative for i>=3 */
            seq_t coeff = (seq_t)(((idx_t)(i-2) * (i-2)) - 1);
            cache[i] = (seq_t)2 * cache[i-1] + coeff * cache[i-2];
        }
    }
    cache_len = n + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
