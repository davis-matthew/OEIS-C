/*
 * A155542  Expansion of 2*(1 + x)/(1 - 8*x - 25*x^2).
 * https://oeis.org/A155542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 18, a(n) = 8*a(n-1) + 25*a(n-2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <vector>
#endif

/* C cache for the linear-time recurrence */
#if !defined(__cplusplus)
static seq_t *cache = NULL;          /* array of computed terms (C only) */
static idx_t cache_cap = 0;          /* allocated capacity */
static idx_t cache_len = 0;          /* highest index computed + 1 */

/* Ensure the C cache has at least `needed+1` slots. */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap > needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
    if (new_cap <= needed)
        new_cap = needed + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* If expanding, the new elements are uninitialized – they will be set below. */
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* --- Sequence function --- */
seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ build: use std::vector for automatic allocation. */
    static std::vector<seq_t> cache_vec;
    /* Determine how many terms we already have. */
    size_t old_size = cache_vec.size();
    if ((idx_t)old_size <= n) {
        cache_vec.resize(n + 1);            /* allocate space up to n */
        /* Compute the missing values. */
        for (idx_t i = old_size; i <= n; ++i) {
            if (i == 0)            cache_vec[i] = (seq_t)2;
            else if (i == 1)      cache_vec[i] = (seq_t)18;
            else                  cache_vec[i] = (seq_t)8 * cache_vec[i-1] + (seq_t)25 * cache_vec[i-2];
        }
    }
    return cache_vec[n];
#else
    /* C build: use a manually managed array. */
    if (n >= cache_len) {
        /* Ensure we have room for indices up to n. */
        ensure_capacity(n);
        /* Compute the missing values. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)           cache[i] = (seq_t)2;
            else if (i == 1)      cache[i] = (seq_t)18;
            else                  cache[i] = (seq_t)8 * cache[i-1] + (seq_t)25 * cache[i-2];
        }
        cache_len = n + 1;
    }
    return cache[n];
#endif
}
