/*
 * A142194  Primes congruent to 29 mod 40.
 * https://oeis.org/A142194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 410, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements OEIS A142194:  
   a(n) = the n‑th prime congruent to 29 (mod 40).  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;    /* cache of found primes */
static idx_t prime_count = 0;        /* number of primes stored */
static idx_t prime_capacity = 0;     /* current cache capacity */
static idx_t next_candidate = 29;    /* next number of the form 40k+29 to test */

seq_t a(idx_t n) {
    /* Lazy initialization of the cache */
    if (prime_cache == NULL) {
        prime_capacity = 16;
        prime_cache = (seq_t *)malloc(sizeof(seq_t) * prime_capacity);
        if (prime_cache == NULL) return 0; /* allocation failure */
    }

    /* Grow the cache until we have at least n primes */
    while (prime_count < n) {
        /* Test the current candidate for primality */
        if (is_prime_big((seq_t)next_candidate)) {
            /* Ensure the cache is large enough */
            if (prime_count >= prime_capacity) {
                prime_capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(prime_cache,
                                              sizeof(seq_t) * prime_capacity);
                if (!tmp) return 0; /* allocation failure */
                prime_cache = tmp;
            }
            prime_cache[prime_count++] = (seq_t)next_candidate;
        }
        /* Proceed to the next number of the form 40k+29 */
        next_candidate += 40;
    }

    return prime_cache[n - 1];
}
