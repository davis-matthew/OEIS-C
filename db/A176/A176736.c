/*
 * A176736  a(n) = (n+9)*a(n-1) + (n-1)*a(n-2), a(-1)=0, a(0)=1.
 * https://oeis.org/A176736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A176736
   Recurrence: a(-1)=0, a(0)=1,
               a(n) = (n+9)*a(n-1) + (n-1)*a(n-2)   for n ≥ 1
   Indexing starts at 0. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cache_len = 0;     /* number of valid entries in cache */
    static size_t cache_cap = 0;     /* current allocated capacity */

    /* Ensure cache has capacity for at least (n+1) entries */
    if (!cache_cap || (size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failure */
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }

    /* Initialize base case if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache_len = 1;
    }

    /* Return cached value if already computed */
    if (n < cache_len) return cache[n];

    /* Compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t coeff1 = (seq_t)(i + (idx_t)9);            /* (i+9)   */
        seq_t coeff2 = (seq_t)(i - (idx_t)1);            /* (i-1)   */
        seq_t prev1  = cache[i - 1];
        seq_t prev2  = (i >= 2) ? cache[i - 2] : (seq_t)0;
        cache[i] = coeff1 * prev1 + coeff2 * prev2;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
