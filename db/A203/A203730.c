/*
 * A203730  Number of (n+1) X 4 0..7 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 99*a(n-1) - 609*a(n-2) - 588*a(n-3) Initial values:
 *            a(1)=216000, a(2)=19071424, a(3)=1754118528 The sequence is
 *            defined for n>=1
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache of computed terms (1‑based indexing stored in 0‑based array) */
    static seq_t *cache      = NULL;      /* array of terms */
    static size_t cache_size = 0;         /* number of terms stored */
    static size_t cache_cap  = 0;         /* array capacity */

    /* Initialize cache with the first three terms */
    if (!cache) {
        /* Allocate space for at least 4 elements (capacity starts at 4) */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed – unlikely in tests */

        cache[0] = 216000ULL;
        cache[1] = 19071424ULL;
        cache[2] = 1754118528ULL;
        cache_size = 3;
    }

    /* Return if the requested term is already cached */
    if ((idx_t)n <= cache_size) {
        return cache[n - 1];
    }

    /* Compute terms up to n, extending the cache as needed */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* a(i-1), a(i-2), a(i-3) are stored at indices i-2, i-3, i-4 */
        seq_t a1 = cache[i - 2];
        seq_t a2 = cache[i - 3];
        seq_t a3 = cache[i - 4];

        seq_t next = (seq_t)99 * a1;
        next -= (seq_t)609 * a2;
        next -= (seq_t)588 * a3;

        /* Grow the cache if necessary */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0; /* allocation failed – unlikely */
        }

        cache[cache_size] = next;
        ++cache_size;
    }

    return cache[n - 1];
}
