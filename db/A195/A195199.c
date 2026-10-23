/*
 * A195199  Smallest multiple of n with more than twice as many divisors as n.
 * https://oeis.org/A195199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min_{k≥1} (k*n) such that τ(k*n) > 2*τ(n)
 * Range:     u8 n <= 35, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* τ counts the positive divisors of an integer.      */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PRIME_LIMIT 200000
#define MAX_PRIMES 30000

static bool prime_flag[PRIME_LIMIT];
static uint32_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;

/* Initialize a simple sieve of Eratosthenes.  */
static void init_primes(void)
{
    if (prime_cnt != 0)   /* already done          */
        return;

    for (size_t i = 0; i < PRIME_LIMIT; ++i)
        prime_flag[i] = false;

    for (uint32_t i = 2; i < PRIME_LIMIT; ++i)
    {
        if (!prime_flag[i])
        {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i < PRIME_LIMIT)
                for (uint32_t j = i * i; j < PRIME_LIMIT; j += i)
                    prime_flag[j] = true;
        }
    }
}

/* Return τ(x) for 0 < x < 2^64 using the pre‑computed primes. */
static seq_t tau_uint64(uint64_t x)
{
    if (x < 2) return 1;   /* τ(1) = 1, τ(0) is not used            */
    seq_t tau = 1;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > x)
            break;

        if (x % p == 0)
        {
            uint64_t exp = 0;
            while (x % p == 0)
            {
                x /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
        }
    }

    if (x > 1)          /* a prime factor larger than √original x */
        tau *= 2;

    return tau;
}

/* Required OEIS function.  */
seq_t a(idx_t n)
{
    if (n == 0)  /* sequence is defined for n≥1; return 0 for safety */
        return 0;

    init_primes();

    uint64_t nn = (uint64_t)n;
    seq_t tau_n = tau_uint64(nn);

    for (idx_t k = 1; ; ++k)
    {
        uint64_t m = nn * (uint64_t)k;
        seq_t tau_m = tau_uint64(m);

        if ((unsigned long long)tau_m > (unsigned long long)tau_n * 2ULL)
            return (seq_t)m;
    }
}
