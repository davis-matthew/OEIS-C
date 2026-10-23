/*
 * A025544  a(n) = sum of the exponents in the prime factorization of the
 *          least common multiple of the first n composite numbers.
 * https://oeis.org/A025544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p <= C_n/2} ⌊log_p(C_n)⌋ * where C_n is the n-th
 *            composite number (first few composites are 4,6,8,9,...)
 * Range:     u8 n <= 1929, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* 1. find the n-th composite number C_n by binary search */
    idx_t low = 4;                     /* first composite */
    idx_t high = n * 4 + 100;          /* safe upper bound for small n */
    idx_t Cn = 0;

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t pi_mid = (idx_t)compute_pi(mid);   /* primes <= mid */
        idx_t composites = mid - pi_mid - 1;     /* composites <= mid */
        if (composites >= n) {
            Cn = mid;
            if (mid == 0) break;        /* avoid infinite loop on overflow */
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    if (Cn == 0) Cn = low;             /* safety: should not happen */

    /* 2. generate all primes up to Cn/2 */
    idx_t limit = Cn / 2;
    if (limit < 2) limit = 2;
    uint64_t *primes = NULL;
    size_t prime_count = 0;
    build_primes_upto(limit, &primes, &prime_count);

    /* 3. sum floor(log_p(Cn)) for each prime p <= limit */
    seq_t total = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        idx_t power = p;
        seq_t exp = 0;
        while (power <= Cn) {
            ++exp;
            /* avoid overflow when multiplying */
            if (power > Cn / p) break;
            power *= p;
        }
        total += exp;
    }

    free(primes);
    return total;
}
