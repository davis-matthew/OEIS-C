/*
 * A170321  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = c_n + 22 * Σ_{k=1}^{min(n,41)} a(n‑k)
 *            – (n ≥ 42 ? 253 * a(n‑42) : 0), where c_n = 2 for 1 ≤ n ≤ 41,
 *            c_0 = 1, and c_n = 0 for n ≥ 42
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Expand or shrink the cache so that it can hold at least n+1 elements. */
static void
ensure_capacity(idx_t n, seq_t **cache, size_t *cap)
{
    size_t needed = (size_t)(n + 1);
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        /* realloc returns void*; cast to the element type. */
        seq_t *ptr = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        *cache = ptr;
        *cap   = new_cap;
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Local cache for computed terms. */
    static seq_t *cache     = NULL;
    static size_t  cache_cap = 0;
    static idx_t   cache_len = 0;          /* number of terms already filled */

    /* Make sure the cache can hold all terms up to n. */
    ensure_capacity(n, &cache, &cache_cap);

    /* Compute any missing terms. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        idx_t upper = (i < 41) ? i : 41;
        for (idx_t k = 1; k <= upper; ++k)
            sum += cache[i - k];

        seq_t term = (i == 0 ? (seq_t)1 : (i <= 41 ? (seq_t)2 : (seq_t)0));
        term += (seq_t)22 * sum;
        if (i >= 42)
            term -= (seq_t)253 * cache[i - 42];

        cache[i] = term;
    }

    /* Record that terms up to n are now available. */
    cache_len = n + 1;
    return cache[n];
}
