/*
 * A089001  Numbers k such that 2*k^2 + 1 is prime.
 * https://oeis.org/A089001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that 2*k^2 + 1 is prime (OEIS
 *            A089001). The routine searches for the next k and caches the
 *            results, so that subsequent calls (in any order) are answered
 *            efficiently
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /*  Cache of the sequence values that have already been found.  */
    static seq_t *cache      = NULL;   /* array of seq_t */
    static size_t cache_size = 0;      /* number of entries stored */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t  last_k      = 0;     /* last k examined for primality */

    /*  a(1) is the first term; undefined for n < 1 – return 0 in that
        (unlikely) case.  */
    if (n == 0) return 0;

    /*  Allocate the cache on first use.  */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();   /* allocation failed */
    }

    /*  Generate terms until we have at least n of them.  */
    while (cache_size < (size_t)n) {
        last_k++;                           /* next candidate k       */
        uint64_t kk = (uint64_t)last_k;      /* work in 64‑bit space  */

        /*  Compute 2*k^2 + 1 using 128‑bit arithmetic to avoid overflow
            for the 64‑bit build.  */
        unsigned __int128 v128 = (unsigned __int128)2 * kk * kk + 1;
        uint64_t v64 = (uint64_t)v128;       /* fits for our expected range */

        /*  Test primality.  The is_prime function is available for
            64‑bit numbers.  For a 128‑bit build the function will be
            supplied as an overload that accepts seq_t.  */
        bool prime = is_prime(v64);

        if (prime) {
            /*  Store the new term, enlarging the cache if necessary.  */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
