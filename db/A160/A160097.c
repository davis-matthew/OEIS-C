/*
 * A160097  Number of non-exponential divisors of n.
 * https://oeis.org/A160097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(n) - A049419(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000005(n) = number of divisors of n (τ(n)) */
/* A049419(n) = number of exponential divisors of n = ∏ τ(e_i) */
/* where n = ∏ p_i^{e_i} */

#include <stdint.h>
#include <stddef.h>

static seq_t divisor_count_uint64(uint64_t m)
{
    seq_t cnt = 1;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t c = 0;
            while (m % p == 0)
            {
                ++c;
                m /= p;
            }
            cnt *= (seq_t)(c + 1);
        }
    }
    if (m > 1)
        cnt *= (seq_t)2;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Factor n into primes and exponents.  We use a simple trial division,
       which is fast enough for the required range of n (≈10^4). */
    uint64_t num = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    for (uint64_t p = 2; p * p <= num; ++p)
    {
        if (num % p == 0)
        {
            uint64_t e = 0;
            while (num % p == 0)
            {
                ++e;
                num /= p;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (num > 1)
    {
        primes[cnt] = num;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Compute τ(n) and product of τ(e_i). */
    seq_t tau = 1;
    seq_t exp_divisor_product = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        tau *= (seq_t)(e + 1);
        exp_divisor_product *= divisor_count_uint64(e);
    }

    return tau - exp_divisor_product;
}
