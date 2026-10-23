/*
 * A142007  Primes congruent to 3 mod 31.
 * https://oeis.org/A142007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime that is congruent to 3 modulo 31.
   It generates candidates of the form 31*k+3 and tests each for primality
   using is_prime.  Found primes are cached for future calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of primes ≡ 3 (mod 31). */
    static seq_t *cache   = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t next_candidate = 3;   /* first integer of the form 31*k+3 */

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Initialise cache capacity if not yet done. */
    if (cache_cap == 0) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Find primes until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (is_prime(next_candidate)) {          /* primality test */
            if (cache_len == cache_cap) {         /* grow cache if full */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;               /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 31;                     /* next candidate of form 3 mod 31 */
    }

    return cache[n - 1];
}
