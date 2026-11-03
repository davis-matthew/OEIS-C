/*
 * A215488  a(0)=1, a(n) = a(n-1) + a(2*n AND n), where AND is the bitwise AND
 *          operator.
 * https://oeis.org/A215488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1, a(n) = a(n-1) + a((2*n) & n)
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 193, u64 n <= 781,
 *            u128 n <= 4029, big any n
 * Validated: standard, pass, 66 terms through n = 9122 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;   /* memoisation cache */
static size_t cap   = 0;      /* current capacity of the cache */
static idx_t  last  = 0;      /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* initialise the cache on first use */
    if (!cache) {
        cap = 1;
        cache = seq_realloc(NULL, 0, cap);
        if (!cache) abort();          /* allocation failure */
        cache[0] = 1;                 /* a(0) = 1 */
        last = 0;
    }

    /* grow the cache if the requested index exceeds the current capacity */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap)
            new_cap = new_cap ? new_cap * 2 : 1;   /* exponential growth */
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing values up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            idx_t m = ((idx_t)2 * i) & i;      /* (2*i) & i */
            cache[i] = cache[i - 1] + cache[m];
        }
        last = n;
    }

    return cache[n];
}
