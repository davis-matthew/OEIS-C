/*
 * A133665  a(n) = a(n-1) - 9*a(n-2), a(0) = 1, a(1) = 3.
 * https://oeis.org/A133665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = 3; a(n) = a(n-1) - 9*a(n-2) for n >= 2
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 40, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_capacity = 0;

    /* ensure cache large enough */
    if (n + 1 > cache_capacity) {
        /* double capacity until big enough or start at n+1 */
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : n + 1;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out‑of‑memory */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* initialise missing starting values */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (cache_size == 1 && n >= 1) {
        cache[1] = (seq_t)3;
        cache_size = 2;
    }

    /* compute up to n if needed */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] - ((seq_t)9 * cache[i-2]);
    }
    if (cache_size <= n) cache_size = n + 1;

    return cache[n];
}
