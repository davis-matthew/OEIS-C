/*
 * A090341  Difference between the sums of the prime factors, including
 *          multiplicity, of n and those of n + 2.
 * https://oeis.org/A090341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001414(n) - A001414(n+2)
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

/* Sum of the prime factors of n, counted with multiplicity. */
static seq_t sum_prime_factors(uint64_t n)
{
    seq_t sum = 0;
    while (n > 1) {
        uint64_t p = smallest_prime_factor(n);
        sum += (seq_t)p;
        n /= p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition; behaviour for n < 1 is unspecified */
    seq_t sum_n   = sum_prime_factors((uint64_t)n);
    seq_t sum_np2 = sum_prime_factors((uint64_t)n + 2);
    return sum_n - sum_np2;
}
