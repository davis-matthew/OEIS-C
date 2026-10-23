/*
 * A198948  Expansion of modular form (E6(q)+8*E6(q^2))/9, where E6 = A013973.
 * https://oeis.org/A198948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -56 * ( σ5(n) + 8 * σ5(n/2) if n is even ) where σ5 is *
 *            the sum of fifth powers of all divisors of n
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

/* sum of fifth powers of divisors of p^e */
static seq_t sigma_prime_power(seq_t p, int e)
{
    seq_t p_pow5 = pow_seq(p, 5);
    seq_t sum = 1;           /* term for exponent 0 */
    seq_t term = 1;
    for (int i = 0; i < e; ++i) {
        term *= p_pow5;
        sum += term;
    }
    return sum;
}

/* σ5(n) = product over prime powers p^e of (1 + p^5 + p^10 + ... + p^(5e)) */
static seq_t sigma5(idx_t n)
{
    if (n == 0) return 0;   /* not used for n == 0 in a(0) */
    seq_t result = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            result *= sigma_prime_power((seq_t)p, e);
        }
    }
    if (tmp > 1) {          /* remaining prime factor with exponent 1 */
        result *= sigma_prime_power((seq_t)tmp, 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* a(0) = 1 */
    seq_t sigma_n = sigma5(n);
    seq_t sigma_half = 0;
    if ((n & 1) == 0) {     /* n even */
        sigma_half = sigma5(n >> 1);
    }
    seq_t total = sigma_n + (seq_t)8 * sigma_half;
    return (seq_t)-56 * total;
}
