/*
 * A075524  Nonprimes of form 4*p + (p mod 4), p prime.
 * https://oeis.org/A075524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th composite number of the form 4*p + (p mod 4)
 *            where p is prime. It is computed by a simple search
 * Range:     u8 n <= 10, u16 n <= 1518, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    /* Static storage for primes and sequence terms.  */
    static uint64_t *primes = NULL;      /* list of primes found so far   */
    static size_t primes_len = 0;        /* number of primes stored       */
    static size_t primes_cap = 0;        /* allocated capacity of primes  */

    static seq_t *terms = NULL;          /* sequence terms                */
    static size_t terms_len = 0;         /* number of terms stored        */
    static size_t terms_cap = 0;         /* allocated capacity of terms   */

    /* Next integer to test for primality.  */
    static uint64_t next_candidate = 2;

    /* Grow the array helpers.  */
    if (primes_cap == 0 && primes == NULL) {
        primes_cap = 32;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
    if (terms_cap == 0 && terms == NULL) {
        terms_cap = 32;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }

    /* Generate terms until we reach the requested index.  */
    while (terms_len < n) {
        /* Test whether next_candidate is prime.  */
        bool isprime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t pr = primes[i];
            if (pr * pr > next_candidate)
                break;
            if (next_candidate % pr == 0) {
                isprime = false;
                break;
            }
        }

        if (isprime) {
            /* Store the new prime.  */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    new_cap * sizeof(uint64_t));
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = next_candidate;

            /* Compute the candidate value 4*p + (p mod 4).  */
            uint64_t p = next_candidate;
            uint64_t cand = 4 * p + (p & 3ULL);   /* p % 4 == p & 3 */

            /* If the candidate is composite, append it to the sequence.  */
            if (!is_prime_u64(cand)) {
                if (terms_len == terms_cap) {
                    size_t new_cap = terms_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(terms,
                                                  new_cap * sizeof(seq_t));
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = (seq_t)cand;
            }
        }

        ++next_candidate;
    }

    return terms[n - 1];
}
