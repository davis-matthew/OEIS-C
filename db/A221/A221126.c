/*
 * A221126  Number of 2 X n arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor.
 * https://oeis.org/A221126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 11*a(n-2) - 12*a(n-3) - 2*a(n-4) for n>6
 *            initial values: a(1)=1, a(2)=21, a(3)=200, a(4)=1950,
 *            a(5)=19005, a(6)=185164 The function computes any term with
 *            memoisation and is O(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[6] = {
    (seq_t)1,
    (seq_t)21,
    (seq_t)200,
    (seq_t)1950,
    (seq_t)19005,
    (seq_t)185164
};

seq_t a(idx_t n)
{
    /* Terms with index ≤ 6 are known directly. */
    if (n <= 6)
        return init_vals[n - 1];

    /* Previously computed terms are kept in a static cache. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* size of the allocated array (1‑based) */
    static idx_t cache_max  = 0;   /* highest n already computed */

    /* Ensure the cache can hold term n. */
    if (cache_size < n + 1) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache)
            abort();          /* memory allocation failed */
        cache = new_cache;
        cache_size = n + 1;
    }

    /* Initialise the first six entries if not already done. */
    if (cache_max < 6) {
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = init_vals[i - 1];
        cache_max = 6;
    }

    /* Compute remaining terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = (seq_t)11 * cache[i - 1]
                  - (seq_t)11 * cache[i - 2]
                  - (seq_t)12 * cache[i - 3]
                  - (seq_t) 2 * cache[i - 4];
        cache[i] = val;
    }
    cache_max = n;

    return cache[n];
}
