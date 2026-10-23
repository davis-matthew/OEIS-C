/*
 * A038615  Primes not containing the digit '7'.
 * https://oeis.org/A038615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 3632, u32 n <= 9679, u64 n <= 9679,
 *            u128 n <= 9679, big any n
 * Helpers:   digit7_in, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine computes the nth prime that does not contain the digit 7
   by brute‑force search.  It uses the supplied is_prime(idx_t) function
   and caches previously found primes so that calls in any order are
   efficient. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"          /* provides bool is_prime(idx_t) */
#include "../utils/digit7_in.h"

static seq_t  *prime_cache      = NULL; /* array of found primes   */
static size_t prime_cache_len   = 0;    /* number of primes stored*/
static size_t prime_cache_cap   = 0;    /* allocated capacity      */
static uint64_t next_candidate  = 2;    /* next number to test     */
static bool     cache_initialized = false;/* first‑time flag       */

seq_t a(idx_t n)
{
    /* Initialize on first call. */
    if (!cache_initialized) {
        prime_cache_cap = 16;
        prime_cache = (seq_t*)malloc(prime_cache_cap * sizeof(seq_t));
        prime_cache_len = 0;
        next_candidate = 2;
        cache_initialized = true;
    }

    /* Generate primes until we have at least n of them. */
    while (prime_cache_len < n) {
        uint64_t cand = next_candidate;
        while (1) {
            /* Only 2 is even; all other candidates are odd. */
            if (cand == 2 || (cand & 1)) {
                if (is_prime((idx_t)cand) && !digit7_in(cand)) {
                    /* Store the prime. */
                    if (prime_cache_len == prime_cache_cap) {
                        prime_cache_cap <<= 1;
                        prime_cache = (seq_t*)realloc(prime_cache,
                                                      prime_cache_cap * sizeof(seq_t));
                    }
                    prime_cache[prime_cache_len++] = (seq_t)cand;
                    /* Next search should begin two numbers higher
                       (to skip the even number just after the prime),
                       except for 2 where we need to start at 3. */
                    next_candidate = (cand == 2) ? 3 : cand + 2;
                    break;
                }
            }
            /* Advance to the next candidate. */
            cand = (cand == 2) ? 3 : cand + 2;
        }
    }

    /* Return the requested term. */
    return prime_cache[(size_t)(n - 1)];
}
