/*
 * A010844  a(n) = 2*n*a(n-1) + 1 with a(0) = 1.
 * https://oeis.org/A010844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a(0) = 1, a(n) = 2*n*a(n-1) + 1  for n ≥ 1  (OEIS A010844) */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_size = 0;  /* number of computed terms */

    /* enlarge cache if needed (use explicit cast for C++ compatibility) */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* simple error handling */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise a(0) if not already done */
    if (cache_size == 0) {
        cache[0] = 1;
        cache_size = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[(size_t)(i - 1)];
        seq_t term = ((seq_t)2 * (seq_t)i) * prev + 1;
        cache[(size_t)i] = term;
    }

    /* update cache size */
    cache_size = (size_t)(n + 1);

    return cache[(size_t)n];
}
