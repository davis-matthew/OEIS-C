/*
 * A065350  a(n) = binomial(2*n, n) mod (n+1)^2.
 * https://oeis.org/A065350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) mod (n+1)^2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int prime_list[20000];
static int prime_cnt = 0;
static int sieve_done  = 0;

/* Pre‑compute primes up to 20000 once */
static void init_primes(void)
{
    if (sieve_done) return;
    int limit = 20000;
    char is_comp[limit + 1];
    for (int i = 2; i <= limit; ++i)
    {
        if (!is_comp[i])
        {
            prime_list[prime_cnt++] = i;
            if ((long long)i * i <= limit)
                for (int j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
        }
    }
    sieve_done = 1;
}

/* Modular exponentiation using unsigned __int128 for safety */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();
    uint64_t m  = (uint64_t)n + 1U;      /* n+1 */
    uint64_t mod = m * m;                /* (n+1)^2  */
    uint64_t res = 1 % mod;
    uint64_t two_n = 2ULL * (uint64_t)n;

    for (int i = 0; i < prime_cnt; ++i)
    {
        int p = prime_list[i];
        if ((uint64_t)p > two_n) break;

        /* Legendre exponent for prime p in C(2n,n) */
        uint64_t e = 0;
        uint64_t t = (uint64_t)p;
        while (t <= two_n) { e += two_n / t; t *= (uint64_t)p; }
        t = (uint64_t)p;
        while (t <= (uint64_t)n) { e -= 2ULL * ((uint64_t)n / t); t *= (uint64_t)p; }

        if (e)
        {
            uint64_t p_pow = powmod_uint64((uint64_t)p, e, mod);
            res = (unsigned __int128)res * p_pow % mod;
        }
    }
    return (seq_t)res;
}
