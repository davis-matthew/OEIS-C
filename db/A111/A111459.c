/*
 * A111459  Generalized Somos-4 sequence with a(n-2)^2 replaced by a(n-2)^5.
 * https://oeis.org/A111459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)*a(n-3) + a(n-2)^5)/a(n-4) for n ≥ 4 with a(0) =
 *            a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static size_t cache_capacity = 0;

/* compute x^5 */
static inline seq_t pow5(seq_t x)
{
    seq_t x2 = x * x;
    seq_t x4 = x2 * x2;
    return x4 * x;
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n < 4)
        return 1;

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache_capacity = 4;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL)
            exit(1);
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = 1;
        cache_size = 4;
    }

    /* ensure capacity for n+1 elements */
    if (n + 1 > cache_capacity) {
        size_t new_cap = cache_capacity;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_ptr == NULL)
            exit(1);
        cache = new_ptr;
        cache_capacity = new_cap;
    }

    /* compute missing terms */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t term = (cache[i - 1] * cache[i - 3] + pow5(cache[i - 2])) / cache[i - 4];
            cache[i] = term;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
