/*
 * A130562  Triangular table of denominators of the coefficients of
 *          Laguerre-Sonin polynomials L(1/2,n,x).
 * https://oeis.org/A130562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A130562 – denominator of Laguerre‑Sonin coefficient
 *  a(n) = denom(L(1/2, r, m)) where n = r*(r+1)/2 + m, 0 ≤ m ≤ r.
 *  The coefficient is  (-1)^m * binomial(r+1/2, r-m) / m! .
 *  Implementation uses prime factorisation of numerator and factorials.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------- helper: Legendre exponent of p in n! ----------------- */
static inline int legendre_exp(idx_t n, idx_t p)
{
    int e = 0;
    while (n)
    {
        n /= p;
        e += (int)n;
    }
    return e;
}

/* -------- helper: generate all primes ≤ limit ------------------- */
static void primes_up_to(idx_t limit, uint64_t **out, size_t *cnt)
{
    bool *sieve = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!sieve) { *out = NULL; *cnt = 0; return; }
    for (size_t i = 0; i <= (size_t)limit; ++i) sieve[i] = true;
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i)
        if (sieve[(size_t)i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[(size_t)j] = false;

    /* count primes */
    size_t pcount = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (sieve[i]) ++pcount;

    uint64_t *pr = (uint64_t *)malloc(pcount * sizeof(uint64_t));
    if (!pr) { free(sieve); *out = NULL; *cnt = 0; return; }
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (sieve[i]) pr[idx++] = (uint64_t)i;

    free(sieve);
    *out = pr;
    *cnt = pcount;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* trivial case n == 0 */
    if (n == 0) return (seq_t)1;

    /* locate r and m : n = r*(r+1)/2 + m, 0≤m≤r */
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n) hi <<= 1;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;
    idx_t m = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = r - m;                 /* number of terms in the binomial */

    /* primes needed: all ≤ 2*r+1 (maximum odd numerator factor) */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    primes_up_to(2 * r + 1, &primes, &prime_cnt);
    if (prime_cnt == 0) return (seq_t)1;   /* safety guard */

    /* exponent of each prime in the numerator product */
    int *num_exp = (int *)calloc(prime_cnt, sizeof(int));
    if (!num_exp) { free(primes); return (seq_t)1; }

    for (idx_t i = 0; i < k; ++i)
    {
        uint64_t val = 2 * r + 1 - 2 * i;        /* odd factor */
        for (size_t pi = 0; pi < prime_cnt; ++pi)
        {
            uint64_t p = primes[pi];
            if (p * p > val) break;
            int cnt = 0;
            while (val % p == 0)
            {
                val /= p;
                ++cnt;
            }
            if (cnt) num_exp[pi] += cnt;
        }
        if (val > 1)   /* remaining prime factor > sqrt */
        {
            for (size_t pi = 0; pi < prime_cnt; ++pi)
            {
                if (primes[pi] == val)
                {
                    num_exp[pi] += 1;
                    break;
                }
            }
        }
    }

    /* compute denominator from prime exponents */
    seq_t result = 1;
    for (size_t pi = 0; pi < prime_cnt; ++pi)
    {
        uint64_t p = primes[pi];
        int den_exp;
        if (p == 2)
        {
            den_exp = (int)k
                      + legendre_exp(k, 2)
                      + legendre_exp(m, 2);
            /* 2 has no numerator factors */
        }
        else
        {
            int e_k = legendre_exp(k, p);
            int e_m = legendre_exp(m, p);
            den_exp = e_k + e_m - num_exp[pi];
        }
        if (den_exp > 0)
        {
            seq_t mul = 1;
            for (int e = 0; e < den_exp; ++e) mul *= (seq_t)p;
            result *= mul;
        }
    }

    free(primes);
    free(num_exp);
    return result;
}
