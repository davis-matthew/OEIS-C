/*
 * A215249  a(n) is the smallest number m >= n such that m^2 + n is a prime.
 * https://oeis.org/A215249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m ≥ n such that m^2 + n is prime
 * Range:     u8 n <= 10, u16 n <= 211, u32 n <= 417, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    for (;; ++m) {
        seq_t val = m * m + n;
        if (is_prime_big(val))
            return m;
    }
}
