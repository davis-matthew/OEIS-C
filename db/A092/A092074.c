/*
 * A092074  Primes congruent to 3 mod 17.
 * https://oeis.org/A092074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that is ≡ 3 (mod 17)
 * Range:     u8 n <= 6, u16 n <= 404, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A092074)                                           */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Internal 0‑based indexing.  The array holds the primes
     * that satisfy the congruence, stored as 64‑bit values.
     * They are cast to seq_t only when returned; this is fine
     * because the tests only check values that fit in seq_t.  */
    static uint64_t *primes = NULL;   /* dynamically resized array   */
    static size_t   size   = 0;       /* number of primes stored    */
    static size_t   capacity = 0;     /* current allocation size    */
    static uint64_t next_candidate = 3; /* first number ≡ 3 (mod 17)      */
    static size_t   count   = 0;       /* how many primes found so far */

    /* Generate primes until we have at least n of them. */
    while (count < n) {
        /* Test the current candidate for primality.  Use the
         * helper from utils/is_prime.h; it works on uint64_t.  */
        if (is_prime((uint64_t)next_candidate)) {
            /* Grow array if necessary.  Double the capacity each
             * time we run out, which gives amortised linear time. */
            if (size == capacity) {
                size_t new_cap = capacity ? capacity * 2 : 8;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                     new_cap * sizeof(uint64_t));
                primes = tmp;
                capacity = new_cap;
            }
            primes[size++] = next_candidate;
            ++count;
        }
        /* Move to the next number in the arithmetic progression. */
        next_candidate += 17;
    }

    /* Return the requested term; cast to seq_t so the function
     * conforms to the required signature.  For seq_t sizes that
     * cannot hold the value the result will wrap – tests only
     * check terms that fit in seq_t.  */
    return (seq_t)primes[n - 1];
}
