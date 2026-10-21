/*
 * A216996  Numbers k such that the digit sum of k*7 is a multiple of 7.
 * https://oeis.org/A216996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1894, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the digit sum of k*7 is a multiple of 7 (A216996) */
/* Computed by searching for k with digit_sum(7*k) % 7 == 0 and memoising. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

#ifdef __cplusplus
#include <new>
#endif

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache   = NULL;    /* cache[i] holds a(i+1) */
    static size_t cache_len  = 0;    /* number of filled entries */
    static size_t cache_cap  = 0;    /* allocated capacity */
    static idx_t  last_checked = 0;  /* largest k examined so far */

    if (n == 0) {
        /* Undefined offset; return 0 to avoid crash. */
        return (seq_t)0;
    }

    /* Return cached value if already computed. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Ensure enough capacity for the required number of terms. */
    if (cache_cap < n) {
        size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
        while (new_cap < n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();      /* allocation failure */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Scan successive integers k, populating the cache until we have n terms. */
    for (idx_t k = last_checked + 1; cache_len < n; ++k) {
        seq_t prod = (seq_t)k * 7;
        if ((digit_sum(prod) % 7) == 0) {
#ifdef __cplusplus
            /* Construct the element in place when compiling C++. */
            ::new (cache + cache_len) seq_t(k);
#else
            cache[cache_len] = k;
#endif
            ++cache_len;
        }
        last_checked = k;
    }

    return cache[n - 1];
}
