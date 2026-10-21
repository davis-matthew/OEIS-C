/*
 * A215027  a(n+1) = (concatenation of n and n+1) - a(n), a(0) = 0.
 * https://oeis.org/A215027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   digit_count_u64, pow10_seq_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; for n ≥ 0, a(n+1) = concat(n , n+1) – a(n)   */
/* (concat(x,y) = x * 10^{digits(y)} + y)                           */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_count_u64.h"
#include "../utils/pow10_seq_u64.h"

/* Return a(n) for the OEIS sequence A215027 */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* first free index in cache */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Initialise cache[0] = 0 on first call */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Grow cache to required size if necessary */
    if ((size_t)n + 1 > cache_len) {
        /* Double capacity until it fits */
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;

        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* Compute missing terms */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i)
        {
            /* Compute concat(i-1, i) = (i-1) * 10^{digits(i)} + i */
            unsigned int d = digit_count_u64((uint64_t)i);
            seq_t pow10 = pow10_seq_u64(d);
            seq_t concat = (seq_t)(i - 1) * pow10 + (seq_t)i;

            cache[i] = concat - cache[i - 1];
        }

        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
