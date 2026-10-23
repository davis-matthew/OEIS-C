/*
 * A039709  a(n) = n-th prime modulo 11.
 * https://oeis.org/A039709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) mod 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is the first prime modulo 11. */
    if (n < 1) return (seq_t)0;   /* value for invalid indices is arbitrary */

    static uint64_t *primes = NULL;   /* array of primes up to current limit */
    static size_t primes_cnt = 0;     /* number of primes stored */
    static idx_t primes_limit = 0;    /* upper bound used to generate primes */

    if (n > primes_cnt) {
        /* Get a safe upper bound for the n‑th prime. */
        idx_t new_limit = _prime_limit_for_idx(n);

        /* Build all primes up to that bound.  The helper allocates
           memory, which we keep for future calls.  */
        build_primes_upto(new_limit, &primes, &primes_cnt);
        primes_limit = new_limit;
    }

    /* Return the n‑th prime mod 11.  primes[n-1] is guaranteed to exist. */
    return (seq_t)(primes[n - 1] % 11);
}
