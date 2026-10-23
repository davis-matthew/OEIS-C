/*
 * A138625  Primes congruent to 12 mod 17.
 * https://oeis.org/A138625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 420, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime of the form 17*k + 12 (primes ≡ 12 (mod 17)) */

#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

static seq_t *prime12_17_cache = NULL;   /* array of cached primes        */
static size_t  prime12_17_cache_cap = 0; /* current storage capacity      */
static idx_t   prime12_17_cache_len = 0; /* number of primes stored       */
static idx_t   prime12_17_last_k   = 0;   /* last k value tested           */

seq_t a(idx_t n)
{
    /* Ensure we have at least n primes cached */
    while (prime12_17_cache_len < n) {
        /* Next k to test, start at 1 */
        prime12_17_last_k++;

        /* Candidate value 17*k + 12 */
        idx_t cand_idx = 17 * prime12_17_last_k + 12;

        /* Test for primality (fits in 64‑bit for all practical n) */
        if (is_prime(cand_idx)) {
            /* Grow cache if necessary */
            grow_seq(&prime12_17_cache, &prime12_17_cache_cap,
                     prime12_17_cache_len + 1);

            /* Store the prime as seq_t */
            prime12_17_cache[prime12_17_cache_len++] =
                (seq_t)cand_idx;
        }
    }

    /* Return the nth prime (1‑based indexing) */
    return prime12_17_cache[n - 1];
}
