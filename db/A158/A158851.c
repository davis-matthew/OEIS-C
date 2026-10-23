/*
 * A158851  a(n) = lcm(1,2,3,...,n) mod (n+1).
 * https://oeis.org/A158851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1,2,...,n) mod (n+1)
 * Range:     u8 n <= 315, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: multiply modulo M safely using 64‑bit arithmetic */
static inline seq_t mulmod(seq_t a, seq_t b, seq_t mod)
{
    unsigned long long ua = (unsigned long long)a;
    unsigned long long ub = (unsigned long long)b;
    unsigned long long um = (unsigned long long)mod;
    return (seq_t)((ua * ub) % um);
}

/* Helper: modular exponentiation */
static inline seq_t powmod(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1)
            result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Prime generation – incremental sieve */
static uint64_t *g_primes = NULL;          /* array of primes found so far   */
static size_t g_primes_cnt = 0;            /* number of primes in g_primes   */
static size_t g_primes_cap = 0;            /* allocated capacity of g_primes */
static idx_t g_primes_max = 1;             /* largest integer for which primes are known */

/* Ensure that all primes ≤ limit are in g_primes */
static void ensure_primes(idx_t limit)
{
    if (limit <= g_primes_max)
        return;

    for (idx_t cand = g_primes_max + 1; cand <= limit; ++cand)
    {
        if (cand < 2)
            continue;

        bool isPrime = true;
        for (size_t i = 0; i < g_primes_cnt; ++i)
        {
            uint64_t p = g_primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0)
            {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
        {
            if (g_primes_cnt == g_primes_cap)
            {
                size_t new_cap = g_primes_cap ? g_primes_cap * 2 : 32;
                uint64_t *tmp = (uint64_t *)realloc(g_primes, new_cap * sizeof(uint64_t));
                if (!tmp)
                    exit(1);
                g_primes = tmp;
                g_primes_cap = new_cap;
            }
            g_primes[g_primes_cnt++] = (uint64_t)cand;
        }
    }
    g_primes_max = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* outside the defined range */

    seq_t mod = (seq_t)(n + 1);

    /* Prepare primes up to n */
    ensure_primes(n);

    seq_t result = 1 % mod;

    for (size_t i = 0; i < g_primes_cnt; ++i)
    {
        uint64_t p = g_primes[i];
        if (p > n)
            break;

        /* Highest power of p not exceeding n */
        idx_t power = p;
        idx_t e = 0;
        while (power <= n)
        {
            ++e;
            if (power > n / p)
                break;
            power *= p;
        }

        seq_t factor = powmod((seq_t)p, e, mod);
        result = mulmod(result, factor, mod);

        if (result == 0)
            break;                     /* early exit if final result is 0 */
    }

    return result;
}
