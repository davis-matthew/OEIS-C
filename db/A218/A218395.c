/*
 * A218395  Numbers whose square is the sum of the squares of 11 consecutive
 *          integers.
 * https://oeis.org/A218395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-2) - a(n-4) * Initial values: a(0)=11, a(1)=77,
 *            a(2)=143, a(3)=1529 * This recurrence applies for all n >= 4.
 *            The function memoizes * previously computed terms so that any
 *            order of calls is fast
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;   /* number of terms currently stored */

    /* initialise cache with the first four terms */
    if (cache_len == 0) {
        size_t init_cap = 4;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache[0] = (seq_t)11;
        cache[1] = (seq_t)77;
        cache[2] = (seq_t)143;
        cache[3] = (seq_t)1529;
        cache_len = 4;
    }

    /* return cached value if already computed */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* ensure we have enough capacity for n+1 elements */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1)) {
            new_cap <<= 1;          /* double capacity until large enough */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();                /* allocation failure; program aborts */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        size_t idx = (size_t)i;
        cache[idx] = (seq_t)20 * cache[idx - 2] - cache[idx - 4];
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
