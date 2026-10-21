/*
 * A052690  Expansion of e.g.f. x*(1+x-3*x^2)/(1-3*x).
 * https://oeis.org/A052690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=8, a(3)=54, a(n)=3*n*a(n-1) for n>=4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for out‑of‑order calls */
    static seq_t *cache   = NULL; /* stored values */
    static idx_t cache_len = 0;   /* number of stored terms (valid indices: 0 .. cache_len-1) */
    static idx_t cache_cap = 0;   /* allocated array size */

    /* Fast path for already computed values */
    if (n < cache_len) {
        return cache[n];
    }

    /* First call: allocate cache with space for the first 4 terms */
    if (cache_len == 0) {
        cache_cap = 8;                     /* over‑allocate a bit for growth */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */

        /* Base terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)54;

        cache_len = 4;
    }

    /* Grow cache if needed */
    while (cache_cap <= n) {
        idx_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence (only for n>=4) */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i >= 4) {
            seq_t prev = cache[i - 1];
            seq_t mult = (seq_t)3 * (seq_t)i; /* 3 * i */
            cache[i] = prev * mult;
        }
        /* For i < 4 the value is already in the base array */
    }

    /* Update length to reflect all computed indices */
    cache_len = n + 1;

    return cache[n];
}
