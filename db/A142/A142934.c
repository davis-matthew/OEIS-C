/*
 * A142934  Primes congruent to 19 mod 64.
 * https://oeis.org/A142934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 204, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th prime congruent to 19 modulo 64.
   The sequence is defined by: a(n) = the n-th prime p such that
   p ≡ 19 (mod 64).  The offset is 1, so a(1) = 19.  The
   implementation builds a sieve of primes up to a fixed limit
   large enough to contain the first 10 000 such primes and then
   selects the required element.  The sieve is built only once
   on the first call. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

#define SIEVE_LIMIT 6000000  /* ample to cover at least 10 000 terms */

static uint64_t *all_primes = NULL;      /* array of all primes up to SIEVE_LIMIT */
static size_t      all_primes_cnt = 0;   /* number of primes in the array */

seq_t a(idx_t n)
{
    /* Validate index: sequence is defined for n >= 1. */
    if (n < 1)
        return 0;   /* undefined, return 0 as sentinel */

    /* Build the sieve once. */
    if (all_primes == NULL) {
        build_primes_upto(SIEVE_LIMIT, &all_primes, &all_primes_cnt);
    }

    /* Find the n-th prime congruent to 19 modulo 64. */
    size_t count = 0;
    for (size_t i = 0; i < all_primes_cnt; ++i) {
        if ((all_primes[i] % 64ULL) == 19ULL) {
            ++count;
            if (count == n) {
                return (seq_t)all_primes[i];
            }
        }
    }

    /* If we reach here, the chosen limit was insufficient.  This
       should not happen for the intended range of n (≤ 10 000).  */
    return 0;
}
