/*
 * A125071  a(n) = sum of the exponents in the prime factorization of n which
 *          are not primes.
 * https://oeis.org/A125071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the exponents in the prime factorization of n *
 *            that are not prime numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_exp.h"

/* Compute A125071 */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    seq_t sum = 0;

    /* Factor out 2 */
    unsigned int exp = 0;
    while ((nn & 1) == 0) {
        nn >>= 1;
        ++exp;
    }
    if (exp > 0 && !is_prime_exp(exp)) sum += (seq_t)exp;

    /* Factor odd primes */
    for (uint64_t p = 3; p <= nn / p; p += 2) {
        if (nn % p == 0) {
            unsigned int e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            if (!is_prime_exp(e)) sum += (seq_t)e;
        }
    }

    /* Remaining prime factor (exponent 1) */
    if (nn > 1) sum += (seq_t)1;

    return sum;
}
