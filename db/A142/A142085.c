/*
 * A142085  Primes congruent to 13 mod 35.
 * https://oeis.org/A142085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p ≡ 13 (mod 35)
 * Range:     u8 n <= 3, u16 n <= 270, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;   /* cache of found primes */
static size_t  cache_cnt  = 0;      /* number of cached primes */
static idx_t   k_next     = 0;      /* next candidate index (p = 35*k + 13) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, but avoid UB */

    /* return from cache if already computed */
    if ((size_t)n <= cache_cnt) {
        return prime_cache[n-1];
    }

    /* grow cache storage if necessary */
    if (prime_cache == NULL) {
        prime_cache = (seq_t*)malloc(n * sizeof(seq_t));
        if (!prime_cache) return 0; /* allocation failure */
    } else if (cache_cnt < (size_t)n) {
        seq_t *tmp = (seq_t*)realloc(prime_cache, n * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        prime_cache = tmp;
    }

    /* generate primes until we have at least n terms */
    while (cache_cnt < (size_t)n) {
        idx_t cand64 = 35 * k_next + 13;          /* candidate as uint64 */
        seq_t cand   = (seq_t)cand64;             /* cast to return type */

        if (is_prime(cand64)) {                  /* test primality */
            prime_cache[cache_cnt++] = cand;     /* store prime */
        }
        ++k_next;                                /* advance to next candidate */
    }

    return prime_cache[n-1];
}
