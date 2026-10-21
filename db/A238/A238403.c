/*
 * A238403  Number of ways a number can be decomposed as a sum of the form pq
 *          + qr + rp where p < q < r are distinct primes.
 * https://oeis.org/A238403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples of distinct primes p < q < r such that
 *            p*q + q*r + r*p = n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* Computes the number of ways to express n as pq + qr + rp with distinct
   primes p < q < r.  Works for any n >= 1. */
seq_t a(idx_t n) {
    /* Small n have no representation. */
    if (n < 31)
        return 0;

    /* Generate all primes <= n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            uint64_t q = primes[j];

            unsigned __int128 prod = (unsigned __int128)p * q;
            if (prod >= n)
                break;                     /* further q only increase prod */

            uint64_t numerator = (uint64_t)(n - (uint64_t)prod);
            uint64_t denom     = p + q;

            if (numerator % denom != 0)
                continue;

            uint64_t r = numerator / denom;
            if (r <= q)          /* ensure r > q for distinctness */
                continue;
            if (!is_prime(r))
                continue;

            /* All conditions satisfied */
            ++count;
        }
    }

    free(primes);
    return count;
}
