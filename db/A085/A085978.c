/*
 * A085978  Number of 4's in decimal expansion of prime(n).
 * https://oeis.org/A085978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_fours_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085978: Number of 4's in the decimal expansion of prime(n) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_fours_uint64.h"

/*--- Prime cache --------------------------------------------------------*/

static uint64_t *prime_cache = NULL;   /* dynamic array of primes          */
static size_t   prime_cnt   = 0;        /* number of primes stored          */
static size_t   prime_cap   = 0;        /* allocated capacity                */
static uint64_t next_candidate = 2;     /* next integer to test for primality */

/* Ensure that prime_cache contains at least n primes.  On first call a
   modest initial capacity is allocated; later calls grow the array
   geometrically.  The routine uses trial division by the already
   known primes. */
static void
ensure_n_primes(idx_t n)
{
    if ((size_t)n <= prime_cnt)
        return;

    /* Grow the cache if necessary. */
    if (prime_cap < (size_t)n) {
        size_t new_cap = n > 16 ? n * 3 / 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (tmp == NULL)            /* allocation failure */
            abort();
        prime_cache   = tmp;
        prime_cap     = new_cap;
    }

    /* Generate primes until we have n of them. */
    while (prime_cnt < (size_t)n) {
        uint64_t cand = next_candidate;

        /* Skip even numbers > 2. */
        if (cand > 2 && (cand & 1ULL) == 0) {
            next_candidate = cand + 1;
            continue;
        }

        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime)
            prime_cache[prime_cnt++] = cand;

        next_candidate = cand + 1;
    }
}

/*--- Count of digit 4 in an unsigned 64-bit integer ---------------------*/

/*--- Sequence function ---------------------------------------------------*/

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe */

    ensure_n_primes(n);
    uint64_t prime_val = prime_cache[n - 1];   /* prime(n)           */
    return (seq_t)count_fours_uint64(prime_val);
}
