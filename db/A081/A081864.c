/*
 * A081864  Sum of 5th powers of the divisors of odd numbers: a(n) =
 *          sigma_5(2n-1).
 * https://oeis.org/A081864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma_5(2n-1) (sum of 5th powers of the divisors of the
 *            odd number 2n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 42, u64 n <= 3565,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint64_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility for computing p^exp as seq_t directly */
#include "../utils/pow_uint64_to_seq.h"

static bool primes_inited = false;
static int primes[20000];
static int prime_cnt = 0;

static void init_primes(void)
{
    if (primes_inited) return;
    const int LIMIT = 20000;
    bool is_prime[LIMIT + 1];
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
    for (int i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[prime_cnt++] = i;
    primes_inited = true;
}

/* Compute sigma_5(m) = sum_{d|m} d^5, with m odd */
static seq_t sigma5(uint64_t m)
{
    seq_t result = 1;
    uint64_t temp = m;
    for (int idx = 0; idx < prime_cnt && (uint64_t)primes[idx] * (uint64_t)primes[idx] <= temp; ++idx)
    {
        int p = primes[idx];
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            seq_t p5 = pow_uint64_to_seq((uint64_t)p, 5);   /* p^5 as seq_t */
            seq_t term = 1;
            seq_t power = 1;
            for (int i = 0; i < e; ++i)
            {
                power *= p5;
                term += power;
            }
            result *= term;
        }
    }
    if (temp > 1)   /* remaining prime factor */
    {
        seq_t p5 = pow_uint64_to_seq(temp, 5);
        result *= (1 + p5);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* behavior below offset is unspecified */
    init_primes();
    uint64_t odd = 2 * (uint64_t)n - 1;
    return sigma5(odd);
}
