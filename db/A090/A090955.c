/*
 * A090955  Numbers in increasing order such that successive sum and
 *          difference both are primes: a(n) + a(n-1) and a(n) - a(n-1) both
 *          are primes.
 * https://oeis.org/A090955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Algorithm: a(1) = 1.
   For n > 1, let prev = a(n-1).  Find the smallest odd prime d such that
   2*prev + d is prime.  Then a(n) = prev + d.
   This guarantees that a(n)-prev = d is prime and
   a(n)+prev = 2*prev + d is prime.

   The function uses lazy memoization and an on‑demand prime sieve.
   Primality checks for sums are performed with the most appropriate
   routine:  is_prime for 64‑bit values and is_prime_big for arbitrary
   precision values.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* --------------------------------------------------------------------- */
/* Helper: test primality of a seq_t value using the correct routine.    */
static inline bool isprime_seq(seq_t v)
{
    /* If seq_t fits in idx_t, use the fast 64‑bit routine. */
    if (sizeof(seq_t) <= sizeof(idx_t)) {
        return is_prime((idx_t)v);
    } else {
        return is_prime_big(v);
    }
}

/* --------------------------------------------------------------------- */
/* Prime generator – keeps a list of all primes found so far.            */
static uint64_t *prime_list = NULL;    /* list of primes (uint64_t)       */
static size_t   prime_len   = 0;       /* number of primes in the list    */
static size_t   prime_cap   = 0;       /* allocated capacity for primes   */
static uint64_t prime_candidate = 2;   /* next integer to test for cubic */

/* Add the next prime to prime_list. */
static void generate_next_prime(void)
{
    for (;;) {
        uint64_t n = prime_candidate++;
        bool isprime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > n) break;
            if (n % p == 0) { isprime = false; break; }
        }
        if (isprime) {
            if (prime_len == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 8;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_len++] = n;
            return;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Cache of computed terms of the sequence.                              */
static seq_t *seq_cache = NULL;   /* array of computed terms          */
static size_t cache_len = 0;      /* number of terms stored          */
static size_t cache_cap = 0;      /* allocated capacity for cache    */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, return 0 */

    /* Ensure the cache can hold the requested term. */
    while (cache_len < (size_t)n) {
        /* Initialise first term 1. */
        if (cache_len == 0) {
            if (cache_cap == 0) {
                cache_cap = 8;
                seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
                if (!seq_cache) abort();
            }
            seq_cache[0] = (seq_t)1;
            cache_len    = 1;
            continue;
        }

        seq_t prev = seq_cache[cache_len - 1];

        /* Ensure at least one prime is available. */
        if (prime_len == 0) generate_next_prime();

        seq_t next_term = 0;
        bool   found    = false;
        size_t i        = 0;

        /* Search for the smallest odd prime d such that
           2*prev + d is prime. */
        while (!found) {
            if (i == prime_len) { generate_next_prime(); continue; }

            seq_t d   = (seq_t)prime_list[i];
            if (d == (seq_t)2) {                       /* skip even prime */
                ++i;
                continue;
            }

            seq_t sum = (seq_t)prev * (seq_t)2 + d;
            if (isprime_seq(sum)) {
                next_term = prev + d;
                found     = true;
                break;
            }
            ++i;
        }

        /* Grow cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(seq_cache,
                                           new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq_cache = tmp;
            cache_cap = new_cap;
        }
        seq_cache[cache_len++] = next_term;
    }

    return seq_cache[n - 1];
}
