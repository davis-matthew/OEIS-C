/*
 * A100683  a(n) = a(n-1) + a(n-2) + a(n-3); a(0) = -1, a(1) = 2, a(2) = 2.
 * https://oeis.org/A100683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3); a(0) = -1, a(1) = 2, a(2) = 2
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 36, i64 n <= 72, i128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_size  = 0;   // number of terms stored (max index + 1)
static idx_t cache_cap   = 0;   // allocated capacity

seq_t a(idx_t n)
{
    /* Initialisation on first call */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;            /* allocation failed */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache_size = 3;                  /* indices 0..2 stored */
    }

    /* Return cached value if requested index is already computed */
    if (n < cache_size) {
        return cache[n];
    }

    /* Ensure enough capacity for the new index */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;                  /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from the current size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
    }
    cache_size = n + 1;     /* new max index + 1 */

    return cache[n];
}
