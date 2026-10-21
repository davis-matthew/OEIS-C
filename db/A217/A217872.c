/*
 * A217872  a(n) = sigma(n)^n.
 * https://oeis.org/A217872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum-of-divisors sigma of n and raises it to the nth power.
   Uses the provided helpers sigma_of for sum of divisors and big_pow for exponentiation. */
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For safety, return 0 for n == 0. */
    if (n == 0)
        return 0;

    /* Compute sigma(n) using the provided helper. */
    seq_t s = sigma_of(n);

    /* Raise sigma(n) to the nth power.  Cast n to seq_t to match big_pow signature. */
    seq_t result = big_pow(s, (seq_t)n);

    return result;
}
