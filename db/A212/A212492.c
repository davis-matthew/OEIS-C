/*
 * A212492  Prime p such that p, p+10, p+12 are all primes.
 * https://oeis.org/A212492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that p, p+10, and p+12 are all
 *            prime (offset 1)
 * Range:     u8 n <= 9, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"

/* Cached primes satisfying the condition. */
static uint64_t *cache64 = NULL;   /* array of found primes (uint64_t) */
static size_t   clen    = 0;        /* number of cached primes */
static size_t   ccap    = 0;        /* current capacity of cache64 */
static uint64_t start   = 2;        /* next candidate prime to test */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but defensive for caller */

    /* Expand the cache until it contains at least n entries. */
    while (clen < n) {
        /* Obtain the next prime ≥ start. */
        uint64_t p = next_prime(start);

        /* Check whether p+10 and p+12 are also prime. */
        if (is_prime((idx_t)(p + 10)) && is_prime((idx_t)(p + 12))) {
            /* Grow the cache if needed. */
            if (ccap == 0) {
                ccap = 64;
                cache64 = (uint64_t*)malloc(ccap * sizeof(uint64_t));
            } else if (clen == ccap) {
                ccap <<= 1;
                cache64 = (uint64_t*)realloc(cache64, ccap * sizeof(uint64_t));
            }
            cache64[clen++] = p;
        }

        /* Move past the prime just examined. */
        start = p + 1;
    }

    /* Return the stored prime as seq_t. For arbitrary-precision seq_t, the cast performs
       an implicit conversion from uint64_t to the big integer type. */
    return (seq_t)cache64[n - 1];
}
