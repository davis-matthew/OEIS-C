/*
 * A147807  Partial sums of A147810(n) = tau(n^2 + 1)/2.
 * https://oeis.org/A147807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{p = 1..n} tau(1 + p^2)/2 (OEIS A147807)
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 2, a(3) = 4, ... */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

static seq_t *cache = NULL;     /* cache[n] holds a(n) for 0 <= n <= cache_len          */
static size_t   cache_cap = 0;  /* number of elements allocated in cache              */
static idx_t    cache_len = 0;  /* highest n for which cache[n] is valid (initially 0) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we simply return 0. */
    if (n == 0)
        return 0;

    /* Ensure the cache can hold indices up to n (including 0). */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)          /* allocation failure – give up */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* 0th entry is the neutral element for the partial sum.          */
    if (cache_len == 0 && cache_cap > 0)
        cache[0] = 0;

    /* Compute missing terms up to n. */
    if (cache_len < n) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            uint64_t p = (uint64_t)i;          /* current p      */
            uint64_t m = p * p + 1ULL;         /* m = p^2 + 1    */
            uint64_t tau = divisor_count_u64(m);/* tau(m)          */
            seq_t term = (seq_t)(tau >> 1);    /* tau/2 (integer) */
            cache[i] = cache[i - 1] + term;    /* running sum     */
        }
        cache_len = n;
    }

    return cache[n];
}
