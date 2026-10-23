/*
 * A134344  Composite numbers such that the arithmetic mean of their prime
 *          factors (counted with multiplicity) is prime.
 * https://oeis.org/A134344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th composite number whose arithmetic mean of prime
 *            factors (counted with multiplicity) is prime
 * Range:     u8 n <= 45, u16 n <= 4483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mean_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mean_prime.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;            /* off‑set 1, undefined below */

    idx_t count = 0;
    uint64_t m = 4;                  /* first composite after 1 */

    while (true) {
        /* factorize m, accumulating sum of prime factors and count k */
        seq_t sum = 0;
        idx_t   k   = 0;

        uint64_t temp = m;
        for (uint64_t p = 2; p * p <= temp; ++p) {
            while (temp % p == 0) {
                sum += (seq_t)p;
                ++k;
                temp /= p;
            }
        }
        if (temp > 1) {               /* remaining prime factor */
            sum += (seq_t)temp;
            ++k;
        }

        /* m is composite iff k > 1 */
        if (k > 1) {
            if (sum % k == 0) {
                seq_t mean = sum / k;
                if (mean_prime(mean)) {
                    ++count;
                    if (count == n) return (seq_t)m;
                }
            }
        }

        ++m;
    }
}
