/*
 * A140938  Number of 6 X 11 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 6,11,n can be permuted, see
 *          formula.
 * https://oeis.org/A140938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product i=0..n-1 ( binom(16+n, 6+i) * i! /
 *            (16+n-i)^(n-i-1) )
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return exponent of prime p in m! */
static idx_t exp_in_fact(idx_t m, uint64_t p)
{
    idx_t e = 0;
    while (m)
    {
        m /= p;
        e += m;
    }
    return e;
}

/* Fast exponentiation for seq_t with an unsigned exponent */
static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t power  = base;
    while (exp)
    {
        if (exp & 1) result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

/* Sieve of Eratosthenes: list of primes up to limit */
static void sieve_primes(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    size_t sz = (size_t)limit + 1;
    bool *comp = (bool*)malloc(sz);
    if (!comp) { *primes_out = NULL; *cnt_out = 0; return; }
    for (size_t i = 0; i < sz; ++i) comp[i] = false;

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i])
        {
            ++cnt;
            for (idx_t j = i + i; j <= limit; j += i) comp[j] = true;
        }

    uint64_t *primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(comp); *primes_out = NULL; *cnt_out = 0; return; }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i]) primes[pos++] = (uint64_t)i;

    *primes_out = primes;
    *cnt_out    = cnt;
    free(comp);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t s = n + 16;                         /* s = 6 + 11 + n - 1 */
    uint64_t *primes;
    size_t pcnt;
    sieve_primes(s, &primes, &pcnt);
    if (!primes) return (seq_t)0;              /* allocation failure */

    /* exponents of each prime in the final product */
    int64_t *exp = (int64_t*)calloc(pcnt, sizeof(int64_t));
    if (!exp) { free(primes); return (seq_t)0; }

    /* exponents of primes in current i! */
    int64_t *fact_exp = (int64_t*)calloc(pcnt, sizeof(int64_t));
    if (!fact_exp) { free(exp); free(primes); return (seq_t)0; }

    /* precompute exponent of each prime in s! */
    int64_t *exp_fact_s = (int64_t*)malloc(pcnt * sizeof(int64_t));
    if (!exp_fact_s) { free(fact_exp); free(exp); free(primes); return (seq_t)0; }
    for (size_t j = 0; j < pcnt; ++j)
        exp_fact_s[j] = (int64_t)exp_in_fact(s, primes[j]);

    for (idx_t i = 0; i < n; ++i)
    {
        idx_t k          = 6 + i;           /* lower index of binomial */
        idx_t s_k        = s - k;
        idx_t exp_factor = n - i - 1;       /* exponent in denominator */

        /* add binomial exponent: s!/(k!(s-k)!) */
        for (size_t j = 0; j < pcnt; ++j)
        {
            int64_t e = exp_fact_s[j]
                        - (int64_t)exp_in_fact(k, primes[j])
                        - (int64_t)exp_in_fact(s_k, primes[j]);
            exp[j] += e;
        }

        /* add factor i! (fact_exp holds exponents of i!) */
        for (size_t j = 0; j < pcnt; ++j)
        {
            if (primes[j] > i) break;
            exp[j] += fact_exp[j];
        }

        /* subtract (s-i)^(exp_factor) */
        if (exp_factor > 0)
        {
            idx_t m = s - i;
            idx_t rem = m;
            for (size_t j = 0; j < pcnt; ++j)
            {
                uint64_t p = primes[j];
                if (p * p > rem) break;
                if (rem % p == 0)
                {
                    int64_t cnt = 0;
                    while (rem % p == 0)
                    {
                        rem /= p;
                        ++cnt;
                    }
                    exp[j] -= cnt * (int64_t)exp_factor;
                }
            }
            if (rem > 1)          /* remaining prime factor */
            {
                for (size_t j = 0; j < pcnt; ++j)
                {
                    if (primes[j] == rem)
                    {
                        exp[j] -= (int64_t)exp_factor;
                        break;
                    }
                }
            }
        }

        /* prepare fact_exp for next i (i+1)! */
        if (i + 1 < n)
        {
            idx_t val = i + 1;
            idx_t rem = val;
            for (size_t j = 0; j < pcnt; ++j)
            {
                uint64_t p = primes[j];
                if (p * p > rem) break;
                if (rem % p == 0)
                {
                    while (rem % p == 0)
                    {
                        fact_exp[j] += 1;
                        rem /= p;
                    }
                }
            }
            if (rem > 1)
            {
                for (size_t j = 0; j < pcnt; ++j)
                {
                    if (primes[j] == rem)
                    {
                        fact_exp[j] += 1;
                        break;
                    }
                }
            }
        }
    }

    /* reconstruct the integer from prime exponents */
    seq_t result = (seq_t)1;
    for (size_t j = 0; j < pcnt; ++j)
    {
        int64_t e = exp[j];
        if (e > 0)
            result *= seq_pow_uint((seq_t)primes[j], (idx_t)e);
    }

    free(primes);
    free(exp);
    free(fact_exp);
    free(exp_fact_s);
    return result;
}
