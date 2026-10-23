/*
 * A133923  a(1)=1, and for n>1, a(n) = a(n-1)/2, if a(n-1) is divisible by 2,
 *          otherwise a(n) = A000005(n*a(n-1)).
 * https://oeis.org/A133923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, if a(n-1) is even a(n) = a(n-1) / 2 else
 *            a(n) = A000005(n * a(n-1)) (i.e. number of divisors of n*a(n-1))
 *            (Reference: OEIS A133923)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* static cache small enough for 10k terms */
    static seq_t *cache     = NULL;   /* array of computed values */
    static size_t cache_len = 0;      /* number of values stored (>= 0) */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;  /* arbitrary start size */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
    }

    /* ensure size 1 value exists (a(1) = 1) */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* if already computed, simply return */
    if (n <= cache_len)
        return cache[n - 1];

    /* compute forward from cache_len+1 up to n,
       enlarging cache if necessary */
    seq_t last = cache[cache_len - 1];
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t cur;
        if ((last & (seq_t)1) == 0) {            /* a(i-1) even */
            cur = last >> 1;
        } else {                                 /* a(i-1) odd */
            /* calculate tau(i * a(i-1)) */
            uint64_t m = (uint64_t)i * (uint64_t)last;
            cur = (seq_t)divisor_count_u64(m);
        }

        /* ensure capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) exit(EXIT_FAILURE);        /* unrecoverable */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store and prepare for next iteration */
        cache[cache_len] = cur;
        cache_len += 1;
        last = cur;
    }

    return cache[n - 1];
}
