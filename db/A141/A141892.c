/*
 * A141892  Primes congruent to 10 mod 21.
 * https://oeis.org/A141892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 550, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes congruent to 10 (mod 21) via incremental search */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/append_candidate.h"

/* cache of primes ≡ 10 (mod 21) */
static seq_t *cache_primes = NULL;
static size_t  cache_len   = 0;   /* number currently stored       */
static size_t  cache_cap   = 0;   /* allocated capacity             */
static idx_t   next_candidate = 31;   /* first candidate ≡10 mod 21 */

/* extend the cache until it contains at least n primes */
static void
extend_cache(idx_t n)
{
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            append_candidate(&cache_primes, &cache_len, &cache_cap,
                             (seq_t)next_candidate);
        }
        next_candidate += 21;   /* keep candidates ≡10 (mod 21) */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for 0; safety */
    if ((size_t)n > cache_len)      /* extend cache if necessary */
        extend_cache((size_t)n);
    return cache_primes[n - 1];
}
