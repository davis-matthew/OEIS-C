/*
 * A102068  a(n) = P(n)!, where P(n) is the largest prime factor of n (with
 *          a(1) = 1).
 * https://oeis.org/A102068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P(n)!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   max_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, otherwise a(n) = (largest prime factor of n)! */
#include <stdint.h>
#include "../utils/max_prime_factor.h"

seq_t a(idx_t n)
{
    seq_t L = max_prime_factor(n);
    seq_t res = 1;

    for (seq_t i = 2; i <= L; ++i)
        res *= i;

    return res;
}
