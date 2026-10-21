/*
 * A062192  Row sums of unsigned triangle A062138 (generalized a=5 Laguerre).
 * https://oeis.org/A062192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 7, a(n) = (2*n+5)*a(n-1) - (n-1)*(n+4)*a(n-2)
 *            (n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* current allocation size */

    /* Initialisation */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_len = 2;
    }

    /* If requested term is already cached */
    if (n < cache_len) return cache[n];

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t prev1 = cache[k-1];
        seq_t prev2 = cache[k-2];

        seq_t term = ((seq_t)2 * (seq_t)k + (seq_t)5) * prev1
                    - ((seq_t)(k-1) * (seq_t)(k+4)) * prev2;

        cache[k] = term;
    }
    cache_len = n + 1;

    return cache[n];
}
