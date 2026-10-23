/*
 * A128215  a(1)=a(2)=1. a(n+1) = a(n) + a(largest prime dividing n).
 * https://oeis.org/A128215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 127, u64 n <= 307,
 *            u128 n <= 709, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A128215: a(1)=a(2)=1,
   a(n+1)=a(n)+a(largest_prime_divisor(n))   for n ≥ 2
   (recurrence is valid for n=2,3,...; the first two terms are given directly)
*/

#include <stdlib.h>
#include "../utils/largest_prime_divisor.h"   /* provides uint64_t largest_prime_divisor(uint64_t) */

seq_t a(idx_t n)
{
    /* cache[0] is unused; cache[1] = a(1), cache[2] = a(2) */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0, cache_len = 0;   /* highest index stored ( ≥ 2 after init ) */

    /* static initialisation: allocate minimal capacity */
    if (cache == NULL) {
        cache_cap = 4;                /* small start */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure is fatal */
        cache[0] = 0;                /* dummy */
        cache[1] = 1;
        cache[2] = 1;
        cache_len = 2;
    }

    /* Extend cache until it contains a(n) */
    while (cache_len < n) {
        idx_t curr = cache_len;   /* current index n in recurrence */
        /* For curr >= 2, obtain its largest prime divisor */
        idx_t lpd = (idx_t)largest_prime_divisor((uint64_t)curr);

        /* Compute a(curr+1) and store it */
        seq_t val = cache[curr] + cache[lpd];

        /* Grow storage if necessary */
        if (cache_len + 1 >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[++cache_len] = val;
    }

    return cache[n];
}
