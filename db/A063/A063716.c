/*
 * A063716  Number of solutions (r,u,s,t), s>=t, in positive integers to the
 *          system of equations 1/r+1/u=1/n, 1/s+1/t=1/u.
 * https://oeis.org/A063716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^2) + Sum_{d|n^2} tau((n+d)^2))/2
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute tau(m^2) = product over prime factors p^e of m of (2*e+1). */
static seq_t tau_square(uint64_t m)
{
    uint64_t pr[20];
    uint64_t ex[20];
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
        res *= (seq_t)(ex[i] * 2 + 1);
    return res;
}

/* Recursively generate all divisors of n^2 and accumulate the sum of
   tau((n + d)^2). */
static void gen_divisors(size_t idx, seq_t cur, seq_t n_val, seq_t *sum,
                         const uint64_t *primes, const uint32_t *exp2, size_t k)
{
    if (idx == k) {
        seq_t m = n_val + cur;
        *sum += tau_square((uint64_t)m);
        return;
    }
    seq_t val = 1;
    for (uint32_t e = 0; e <= exp2[idx]; ++e) {
        gen_divisors(idx + 1, cur * val, n_val, sum, primes, exp2, k);
        val *= (seq_t)primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 by the sequence definition */
    if (n == 0) return 0;

    /* factorize n */
    uint64_t primes[20];
    uint64_t exp[20];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exp, &cnt);

    /* tau(n^2) and doubled exponents for divisor generation */
    seq_t tau_n2 = 1;
    uint32_t exp2[20];
    for (size_t i = 0; i < cnt; ++i) {
        tau_n2 *= (seq_t)(exp[i] * 2 + 1);
        exp2[i] = (uint32_t)(exp[i] * 2);
    }

    /* sum over all divisors d of n^2 of tau((n+d)^2) */
    seq_t sum = 0;
    gen_divisors(0, (seq_t)1, (seq_t)n, &sum, primes, exp2, cnt);

    return (tau_n2 + sum) / 2;
}
