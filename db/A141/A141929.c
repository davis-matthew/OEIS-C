/*
 * A141929  Primes congruent to 3 mod 25.
 * https://oeis.org/A141929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are primes congruent to 3 (mod 25)
 * Range:     u8 n <= 3, u16 n <= 326, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 3, a(2) = 53, a(3) = 103, a(4) = 353, … */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

/* cache of primes ≡ 3 (mod 25) */
static seq_t *prime_mod3arr = NULL;   /* array of cached terms   */
static size_t  prime_mod3cap   = 0;    /* current capacity      */
static size_t  prime_mod3len   = 0;    /* number of cached terms */
static uint64_t next_candidate = 3;    /* first candidate (3 mod 25) */

/* Return the nth term (n ≥ 1).  Calls may arrive in any order. */
seq_t
a(idx_t n)
{
    if (n == 0)               /* offset is 1; guard against illegal value */
        return 0;

    /* Generate new terms until we have at least n cached */
    while (prime_mod3len < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;

        /* Test primality */
        if (isPrimeGeneric(cand)) {
            /* Grow the array if needed */
            if (prime_mod3len == prime_mod3cap) {
                size_t new_cap = (prime_mod3cap == 0) ? 128
                                                : prime_mod3cap * 2;
                seq_t *tmp = (seq_t *)realloc(prime_mod3arr,
                                               new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();                     /* allocation failure */
                prime_mod3arr = tmp;
                prime_mod3cap = new_cap;
            }
            prime_mod3arr[prime_mod3len++] = cand;
        }

        next_candidate += 25;   /* next integer ≡ 3 (mod 25) */
    }

    /* Return the requested term */
    return prime_mod3arr[(size_t)n - 1];
}
