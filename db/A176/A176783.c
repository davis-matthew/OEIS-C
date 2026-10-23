/*
 * A176783  Primes of the form 13*n^2+3*n+1.
 * https://oeis.org/A176783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime of the form 13*k^2 + 3*k + 1 (nth such prime)
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t last_poly_index = 0;

seq_t a(idx_t n)
{
    /* The offset is 1; guard against n == 0 just in case */
    if (n == 0) return 0;

    /* Extend the cache until it contains at least n elements */
    while (cache_len < n) {
        /* Advance to the next argument of the polynomial */
        last_poly_index++;

        /* Compute the polynomial value 13*k^2 + 3*k + 1 */
        seq_t k   = (seq_t)last_poly_index;
        seq_t val = k * k * 13 + k * 3 + 1;

        /* Test whether the value is prime (fits in idx_t for the
           range of n that will be requested by the tests) */
        if (is_prime((idx_t)val)) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();              /* out of memory */
                prime_cache = tmp;
            }
            prime_cache[cache_len++] = val;
        }
    }
    return prime_cache[n - 1];
}
