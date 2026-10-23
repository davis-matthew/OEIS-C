/*
 * A154356  Primes of the form 25n^2-14n+2 for n >= 0.
 * https://oeis.org/A154356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 13, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Prime of polynomial 25(n-1)^2-14(n-1)+2, nth prime in order   */
/*  a(1) = 2, a(2) = 13, a(3) = 557, …                          */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;   /* cached prime values          */
static size_t   cache_size  = 0;    /* number of primes stored      */
static size_t   cache_cap   = 0;    /* capacity of prime_cache       */
static idx_t    next_m      = 0;    /* next polynomial index to test*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not in the sequence domain */

    /* Ensure we have at least n primes in the cache. */
    while (cache_size < n) {
        idx_t    m  = next_m;
        seq_t    mm = (seq_t)m;
        seq_t    val = 25 * mm * mm - 14 * mm + 2;

        if (is_prime_big(val)) {
            /* Grow cache if needed. */
            if (cache_size >= cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                prime_cache = (seq_t *)realloc(prime_cache,
                                               cache_cap * sizeof(seq_t));
            }
            prime_cache[cache_size++] = val;
        }
        ++next_m;
    }
    return prime_cache[n-1];
}
