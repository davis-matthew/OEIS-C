/*
 * A180150  Numbers n such that n and n+2 are both divisible by exactly 4
 *          primes (counted with multiplicity).
 * https://oeis.org/A180150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 2547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A180150: a(n) is the n-th integer m such that bigomega(m)=bigomega(m+2)=4.
   Computed by a linear search with caching. */

#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* static state to cache previously computed terms */
    static seq_t *cache = NULL;         /* array of found terms */
    static size_t cache_cap = 0;        /* allocated capacity of cache */
    static size_t cache_len = 0;        /* number of terms stored in cache */

    static idx_t cur_scan = 0;          /* last number scanned (exclusive) */
    static idx_t found_cnt = 0;         /* number of terms found so far */

    /* if we already have the requested term, return it immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* ensure capacity for new terms */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* scan forward until we have found the n-th term */
    while (found_cnt < n) {
        ++cur_scan;                     /* next candidate integer */
        if (bigomega(cur_scan) == (seq_t)4 &&
            bigomega(cur_scan + (idx_t)2) == (seq_t)4) {
            /* found a valid term, store it */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed: abort for safety */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur_scan;
            ++found_cnt;
        }
    }

    return cache[n - 1];
}
