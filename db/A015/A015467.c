/*
 * A015467  q-Fibonacci numbers for q=9, scaling a(n-2).
 * https://oeis.org/A015467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 9^(n-2) * a(n-2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 0, a(1) = 1 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;            /* highest index already computed */
    static seq_t pow9 = (seq_t)0;         /* 9^(current_index - 2) for next term */

    /* First call: initialise base cases and allocate storage */
    if (!init) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – unlikely */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        pow9    = (seq_t)1;           /* 9^(2-2) for index 2 */
        computed = 1;
        init = true;
    }

    /* Return cached value if already computed */
    if (n <= (idx_t)(computed)) {
        return cache[n];
    }

    /* Ensure the cache is large enough for index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t term = cache[i - 1] + pow9 * cache[i - 2];
        cache[i] = term;
        pow9    = pow9 * (seq_t)9;
    }
    computed = n;
    return cache[n];
}
