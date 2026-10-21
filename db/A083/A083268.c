/*
 * A083268  a(n) is the lcm of related numbers to n (counted in A073757):
 *          related = {divisor-set, RRS}.
 * https://oeis.org/A083268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, A038610(n)) A038610(n) is the set of integers ≤ n
 *            that are coprime to n (including 1). Thus a(n) is the least
 *            common multiple of n together with all numbers ≤ n that are
 *            coprime to n. It equals: product over all primes p ≤ n of p^e_p,
 *            where e_p = exponent of p in n if p | n floor(lo...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Pre‑compute all primes up to PRIME_LIMIT. */
#define PRIME_LIMIT 20000
static bool sieve[PRIME_LIMIT + 1];
static uint32_t primes[2500];   /* π(20000)=2262 < 2500 */
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
    {
        if (sieve[i])
        {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[prime_cnt++] = (uint32_t)i;
    primes_inited = true;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety; not used */
    init_primes();

    /* Factorise n.  exps[i] holds the exponent of primes[i] in n. */
    static int exps[2500];
    for (size_t i = 0; i < prime_cnt; ++i) exps[i] = 0;

    idx_t tmp = n;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)tmp) break;
        if (tmp % p == 0)
        {
            int cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
            }
            exps[i] = cnt;
        }
    }
    if (tmp > 1)      /* remaining prime factor > sqrt(old n) */
    {
        /* Find its index in primes list */
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            if (primes[i] == (uint32_t)tmp)
            {
                exps[i] = 1;
                break;
            }
        }
    }

    seq_t result = (seq_t)1;

    /* Build the product over all primes ≤ n */
    for (size_t i = 0; i < prime_cnt && primes[i] <= n; ++i)
    {
        uint32_t p = primes[i];
        int e = exps[i];
        if (e > 0)            /* p divides n: use its exponent in n */
        {
            seq_t power = 1;
            for (int k = 0; k < e; ++k)
                power *= (seq_t)p;
            result *= power;
        }
        else                  /* p does not divide n: use largest power ≤ n */
        {
            seq_t pk = (seq_t)p;
            while (pk <= (seq_t)n / (seq_t)p)
                pk *= (seq_t)p;
            result *= pk;
        }
    }

    return result;
}
