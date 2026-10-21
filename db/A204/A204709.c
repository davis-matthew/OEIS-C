/*
 * A204709  Number of (n+1) X 5 0..1 arrays with the permanents of all 2 X 2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 13*a(n-2) + 3*a(n-3) - 16*a(n-4) with a(1) = 49,
 *            a(2) = 209, a(3) = 833, a(4) = 3473
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* storage for computed terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t computed = 0;       /* highest n for which a(n) is computed */

    if (n < 1) return 0;             /* undefined for n < 1, return 0 */

    /* Ensure the cache is large enough to hold indices up to n */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1)          cache[i - 1] = (seq_t)49;
        else if (i == 2)     cache[i - 1] = (seq_t)209;
        else if (i == 3)     cache[i - 1] = (seq_t)833;
        else if (i == 4)     cache[i - 1] = (seq_t)3473;
        else {
            /* a(i) = a(i-1) + 13*a(i-2) + 3*a(i-3) - 16*a(i-4) */
            seq_t v1 = cache[i - 2];                   /* a(i-1) */
            seq_t v2 = cache[i - 3];                   /* a(i-2) */
            seq_t v3 = cache[i - 4];                   /* a(i-3) */
            seq_t v4 = cache[i - 5];                   /* a(i-4) */
            cache[i - 1] = v1 + (seq_t)13 * v2 + (seq_t)3 * v3
                            - (seq_t)16 * v4;
        }
    }
    computed = n;
    return cache[n - 1];
}
