/*
 * A023650  Convolution of composite numbers and odd numbers.
 * https://oeis.org/A023650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} composite(k) * odd(n-k+1) where composite(k)
 *            is the k-th composite number (4,6,8,9,10,…) and odd(m) = 2*m-1
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    /*--------------------------------------------------------------------
     *  Static buffers for composite numbers and the sequence cache.
     *--------------------------------------------------------------------*/
    static seq_t *comps   = NULL;   /* composite numbers */
    static size_t comps_cap = 0;
    static size_t comps_sz  = 0;    /* current number of composites */

    static seq_t *cache   = NULL;   /* a(1)..a(cache_sz) */
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;

    /*--------------------------------------------------------------------
     *  Expand composite list until we have at least n composites.
     *--------------------------------------------------------------------*/
    while (comps_sz < n) {
        seq_t cand = (comps_sz == 0) ? (seq_t)4
                                     : comps[comps_sz - 1] + 1;
        /* skip to next composite */
        while (true) {
            if (cand >= (seq_t)4 && !is_prime_small(cand))
                break;
            cand++;
        }

        /* grow buffer if needed */
        if (comps_sz == comps_cap) {
            size_t new_cap = comps_cap == 0 ? 16 : comps_cap * 2;
            seq_t *tmp = (seq_t *)realloc(comps, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – terminate */ abort(); }
            comps = tmp;
            comps_cap = new_cap;
        }
        comps[comps_sz++] = cand;
        cand++;   /* next candidate for next iteration */
    }

    /*--------------------------------------------------------------------
     *  Compute sequence values up to nth term, storing into cache.
     *--------------------------------------------------------------------*/
    while (cache_sz < n) {
        idx_t i = cache_sz + 1;      /* current n to compute */
        seq_t sum = 0;

        for (idx_t k = 1; k <= i; ++k) {
            seq_t c   = comps[k - 1];                 /* composite(k)   */
            seq_t odd = (seq_t)2 * (i - k + 1) - 1;   /* odd(n-k+1)      */
            sum += c * odd;
        }

        /* grow cache buffer if needed */
        if (cache_sz == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – terminate */ abort(); }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_sz++] = sum;
    }

    return cache[n - 1];
}
