/*
 * A144141  a(n) = Hermite(n,2).
 * https://oeis.org/A144141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Hermite(n,2) Recurrence: a(0)=1, a(1)=4, a(n)=4*a(n-1) -
 *            2*(n-1)*a(n-2) (n>=2) Derived from
 *            H_{n+1}(x)=2xH_n(x)-2nH_{n-1}(x) with x=2
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 16, i64 n <= 27, i128 n <= 48,
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
    static idx_t cache_size = 0;   /* number of elements stored */

    /* initialise cache with first two values */
    if (!cache) {
        seq_t *p = (seq_t *)realloc(NULL, sizeof(seq_t) * 2);
        if (!p) return (seq_t)0; /* out‑of‑memory fallback */
        cache = p;
        cache[0] = (seq_t)1; /* H_0(2) */
        cache[1] = (seq_t)4; /* H_1(2) */
        cache_size = 2;
    }

    if (n < cache_size)               /* already computed value */
        return cache[n];

    /* grow cache to hold a(n) */
    idx_t new_size = n + 1;
    seq_t *p = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_size);
    if (!p) return (seq_t)0;          /* out‑of‑memory fallback */
    cache = p;
    /* compute values from the old size up to n inclusive */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - 2*(i-1)*a(i-2) */
        seq_t mult = (seq_t)(2 * (i - 1));
        cache[i] = (seq_t)4 * cache[i - 1] - mult * cache[i - 2];
    }
    cache_size = new_size;
    return cache[n];
}
