/*
 * A169227  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter group
 *            * on 22 generators with relations S_i^2 = (S_i S_j)^28 = I. * *
 *            Recurrence: * a(0) = 1 * for 1 ≤ n ≤ 27: * a(n) = 2 + 20 *
 *            Σ_{i=0}^{n-1} a(i) * for n ≥ 28: * a(n) = 20 * Σ_{i=n-27}^{n-1}
 *            a(i) – 210 * a(n-28)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached values ------------------------------------------------------------ */
static seq_t *cache = NULL;   /* array of computed a(n) */
static idx_t  cache_len = 0;  /* number of elements currently stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* Ensure that cache holds values up to at least n (inclusive).              */
static void
ensure_cache(idx_t n)
{
    /* Allocate initial capacity if needed. */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Grow capacity if n exceeds current capacity. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new values as needed. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i <= 27) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < i; ++j) sum += cache[j];
            cache[i] = (seq_t)2 + sum * (seq_t)20;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t j = i - 27; j < i; ++j) sum += cache[j];
            cache[i] = sum * (seq_t)20 - (seq_t)210 * cache[i - 28];
        }
    }
    cache_len = n + 1;
}

/* Main sequence function --------------------------------------------------- */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
