/*
 * A078024  Expansion of (1-x)/(1-2*x^2-x^3).
 * https://oeis.org/A078024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-3), with a(0)=1, a(1)=-1, a(2)=2
 * Range:     i8 n <= 13, i16 n <= 25, i32 n <= 48, i64 n <= 94, i128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed = 0;

seq_t a(idx_t n)
{
    /* Ensure cache is initialized with the first three terms */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unexpected OOM, just return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)2;
        computed = 3;
    }

    /* If the requested index is already computed, return it directly */
    if (n < computed)
        return cache[n];

    /* Extend cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = cache_len * 2;
        if (new_len < n + 1) new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* unexpected OOM, just return 0 */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute terms from the first missing index up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* Recurrence: a[i] = 2*a[i-2] + a[i-3] */
        cache[i] = (seq_t)2 * cache[i - 2] + cache[i - 3];
    }

    computed = n + 1;
    return cache[n];
}
