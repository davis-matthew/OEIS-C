/*
 * A071625  Number of distinct exponents when n is factorized as a product of
 *          primes.
 * https://oeis.org/A071625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct exponents in the prime factorization
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count the distinct exponents in the prime factorization of n.
   n is at least 1. For n = 1 the value is 0. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* exponents cannot exceed 64 for 64‑bit n, so a modest array suffices */
    bool seen[70] = {0};
    seq_t distinct = 0;

    idx_t tmp = n;
    for (idx_t i = 2; i <= tmp / i; ++i) {
        if (tmp % i == 0) {
            idx_t exp = 0;
            while (tmp % i == 0) {
                tmp /= i;
                ++exp;
            }
            if (!seen[exp]) {
                seen[exp] = true;
                ++distinct;
            }
        }
    }

    /* Remaining prime factor, if any, has exponent 1 */
    if (tmp > 1 && !seen[1]) {
        seen[1] = true;
        ++distinct;
    }

    return (seq_t)distinct;
}
