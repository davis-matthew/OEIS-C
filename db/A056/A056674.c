/*
 * A056674  Number of squarefree divisors which are not unitary. Also number
 *          of unitary divisors which are not squarefree.
 * https://oeis.org/A056674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^ω(n) – 2^r(n) where ω(n) is the number of distinct
 *            prime factors of n, and r is the number of primes that occur to
 *            the first power in the prime factorisation of n. This counts the
 *            squarefree divisors of n that are not unitary (equivalently the
 *            unitary divisors that are not squarefree)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9564 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline void factor_distinct_exponents(idx_t n, idx_t *omega, idx_t *r)
{
    idx_t x = n;
    *omega = 0;
    *r = 0;
    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            ++(*omega);
            idx_t exp = 0;
            while (x % p == 0)
            {
                x /= p;
                ++exp;
            }
            if (exp == 1)
                ++(*r);
        }
    }
    if (x > 1) /* remaining prime factor */
    {
        ++(*omega);
        ++(*r);   /* exponent is 1 */
    }
}
#ifndef NULL
#define NULL ((void*)0)
#endif

seq_t a(idx_t n)
{
    idx_t omega = 0, r = 0;
    if (n >= 1)
    {
        factor_distinct_exponents(n, &omega, &r);
    }
    /* compute powers of two; bounds are small (omega <= ~5 for n <= 10^4) */
    seq_t pow_omega = (seq_t)1 << omega;
    seq_t pow_r     = (seq_t)1 << r;
    return pow_omega - pow_r;
}
