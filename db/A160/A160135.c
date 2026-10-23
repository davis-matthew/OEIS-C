/*
 * A160135  Sum of non-exponential divisors of n.
 * https://oeis.org/A160135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(n) - A051377(n)
 * Range:     u8 n <= 16, u16 n <= 1764, u32 n <= 9673, u64 n <= 9673,
 *            u128 n <= 9673, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* small integer power: base^exp for base≤2^64, exp≤64 */
static inline seq_t pow_seq_uint64(uint64_t base, uint64_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* a(1) = 1 */

    uint64_t temp = (uint64_t)n;
    seq_t sigma   = 1;
    seq_t exp_sum = 1;            /* product of sums of p^d over d|e */

    /* trial division factorisation */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* contribution to σ(n) */
            seq_t p_pow_e1   = pow_seq_uint64(p, e + 1);
            seq_t term_sigma = (p_pow_e1 - 1) / (p - 1);
            sigma *= term_sigma;

            /* contribution to the sum of exponential divisors */
            seq_t sum_div = 0;
            for (uint64_t d = 1; d <= e; ++d)
                if (e % d == 0)
                    sum_div += pow_seq_uint64(p, d);
            exp_sum *= sum_div;
        }
    }

    /* remaining prime factor > sqrt(temp) */
    if (temp > 1)
    {
        uint64_t p = temp;
        uint64_t e = 1;
        seq_t p_pow_e1   = pow_seq_uint64(p, e + 1);   /* e+1 == 2 */
        seq_t term_sigma = (p_pow_e1 - 1) / (p - 1);
        sigma *= term_sigma;
        exp_sum *= pow_seq_uint64(p, 1);              /* p^1 */
    }

    return sigma - exp_sum;
}
