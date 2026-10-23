/*
 * A162806  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1, a(1) = 20, a(2) = 380, a(3) = 7030 * For n ≥ 4: a(n)
 *            = 18*a(n‑1) + 18*a(n‑2) - 171*a(n‑3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache for the sequence values */
static seq_t *cache = NULL;          /* array of computed a(n) values */
static idx_t cache_size = 0;         /* number of values currently stored */
static size_t cache_cap = 0;         /* current allocated capacity */

/* Ensure the cache contains values up to index `n` (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                 /* nothing to do */

    /* Grow the cache until it can hold index `n` */
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;          /* double until large enough */

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    /* Compute any missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)20;
        else if (i == 2)
            cache[i] = (seq_t)380;
        else if (i == 3)
            cache[i] = (seq_t)7030;
        else
            cache[i] = (seq_t)18 * cache[i-1] + (seq_t)18 * cache[i-2] - (seq_t)171 * cache[i-3];
    }

    cache_size = n + 1;
}

/* Return the n-th term of A162806 */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
