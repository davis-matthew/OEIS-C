/*
 * A078835  Sum of the divisors of the average of consecutive odd primes.
 * https://oeis.org/A078835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma_of( (odd_prime(n) + odd_prime(n+1)) / 2 ) where
 *            odd_prime(k) is the k‑th odd prime (3,5,7,11,…). The offset is
 *            1. This implementation generates a list of primes up to a
 *            conservative upper bound using build_primes_upto, extracts the
 *            required consecutive odd primes, averages them, and...
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; offset 1, so n==0 is never used, but
       we guard against it. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Conservative upper bound on the (n+2)-th prime (the
       (n+1)-th odd prime).  For n up to 10 000 a factor of 20
       is more than enough: 20·n+100 > p_{n+2}.  This avoids
       floating point arithmetic. */
    idx_t limit = (idx_t)n * 20ULL + 100ULL;
    if (limit < 10ULL) limit = 10ULL;      /* safety for tiny n */

    /* Generate all primes <= limit.  The routine allocates an array
       of uint64_t primes and sets prime_count.  Caller must free. */
    uint64_t *all_primes = NULL;
    size_t prime_count = 0;
    build_primes_upto(limit, &all_primes, &prime_count);

    /* Find the n‑th and (n+1)‑th odd primes.  odd_idx counts only
       primes > 2.  The first odd prime (3) has odd_idx == 0. */
    size_t odd_idx = 0;
    uint64_t p1 = 0, p2 = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        if (all_primes[i] == 2ULL) {
            continue;                    /* skip the prime 2 */
        }
        if (odd_idx == (size_t)n - 1) {
            p1 = all_primes[i];
        }
        if (odd_idx == (size_t)n) {
            p2 = all_primes[i];
            break;                       /* both primes found */
        }
        ++odd_idx;
    }

    /* Free the array allocated by build_primes_upto. */
    free(all_primes);

    /* Integer average of the two odd primes.  For odd primes the
       sum is even, so division yields an integer. */
    uint64_t avg = (p1 + p2) >> 1U;   /* same as /2 */

    /* Return sigma(avg).  The sigma_of routine accepts idx_t and
       returns seq_t, so we cast avg to idx_t. */
    return sigma_of((idx_t)avg);
}
