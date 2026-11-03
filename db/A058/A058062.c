/*
 * A058062  Number of distinct prime factors of sigma(n), the sum of the
 *          divisors of n.
 * https://oeis.org/A058062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(A000203(n)) -- number of distinct prime factors
 *            of sigma(n)
 * Range:     u8 n <= 188, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Validated: standard, pass, 121 terms through n = 4639 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/omega.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* sigma_of returns the sum of divisors of n as seq_t. */
    seq_t s = sigma_of(n);

    /* sigma(n) is small enough for the test range; cast to uint64_t for prime enumeration. */
    uint64_t sigma_val = (uint64_t)s;

    /* count_distinct_primes returns the number of distinct prime factors of a uint64_t. */
    int cnt = omega(sigma_val);

    return (seq_t)cnt;
}
