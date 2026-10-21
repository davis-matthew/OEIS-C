/*
 * A076290  Sum of the semiprime divisors of n.
 * https://oeis.org/A076290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the semiprime divisors of n (a semiprime is a
 *            product of two primes, possibly equal)
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;  /* 1 has no semiprime divisors */

    uint64_t x = (uint64_t)n;
    uint64_t fac[20];
    int exp[20];
    int cnt;

    /* factor n into primes fac[i]^exp[i] */
    factor_uint64(x, fac, exp, &cnt);

    seq_t sum = 0;

    /* add p^2 for each prime with exponent >= 2 */
    for (int i = 0; i < cnt; ++i) {
        if (exp[i] >= 2) {
            seq_t p = (seq_t)fac[i];
            sum += p * p;
        }
    }

    /* add p*q for each unordered pair of distinct primes occurring in n */
    for (int i = 0; i < cnt; ++i) {
        if (exp[i] >= 1) {
            seq_t p = (seq_t)fac[i];
            for (int j = i + 1; j < cnt; ++j) {
                if (exp[j] >= 1) {
                    seq_t q = (seq_t)fac[j];
                    sum += p * q;
                }
            }
        }
    }

    return sum;
}
