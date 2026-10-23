/*
 * A129755  Triangular numbers t such that t+10 is a prime.
 * https://oeis.org/A129755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth triangular number t such that t+10 is a prime
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 4451, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(1) = 1, a(2) = 3, ... as given in OEIS A129755. */
/* The implementation uses a memoised cache so that calls in any
   order are fast and deterministic. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms cached */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static idx_t next_m = 1;       /* next m to test for t = m*(m+1)/2 */

    if (n == 0)          /* offset 1; n>=1 in correct usage */
        return 0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* compute t = m*(m+1)/2 in seq_t arithmetic */
        seq_t t = (seq_t)next_m * ((seq_t)next_m + 1) / 2;

        /* t+10 is small enough to fit in idx_t (it will be < 2^63 for
           the range of n we target).  Convert to idx_t for prime test. */
        idx_t value = (idx_t)(t + 10);

        if (is_prime(value)) {
            /* enlarge cache if necessary */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = t;
        }
        ++next_m;
    }

    return cache[n-1];
}
