/*
 * A054509  Log_b 6 where b is smallest primitive root (A001918) mod n-th
 *          prime.
 * https://oeis.org/A054509
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 6678, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Determination of A054509:  a(n) = discrete logarithm of 6 base g modulo p_n
 * where p_n is the n‑th prime and g is the smallest primitive root modulo p_n.
 * For n < 4 the function returns 0 (outside the OEIS offset).
 * The implementation uses trial‑division factorisation of p−1 and
 * baby‑step giant‑step (BSGS) for the discrete logarithm.
 * All arithmetic stays within 64‑bit unsigned integers.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "../utils/get_prime.h"

static uint64_t
pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1ULL;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return res;
}

/* Return the distinct prime factors of `n` (n > 1).  Count is written
 * to `cnt`.  The caller must provide arrays of at least 64 elements. */
static void
distinct_prime_factors(uint64_t n, uint64_t *factors, size_t *cnt)
{
    size_t i = 0;
    for (uint64_t p = 2ULL; p * p <= n; ++p)
    {
        if (n % p == 0ULL)
        {
            factors[i++] = p;
            while (n % p == 0ULL)
                n /= p;
        }
    }
    if (n > 1ULL)
        factors[i++] = n;
    *cnt = i;
}

/* Smallest primitive root of a prime `p`.  `p` must be > 2. */
static uint64_t
smallest_primitive_root(uint64_t p)
{
    uint64_t phi = p - 1ULL;
    uint64_t factors[64];
    size_t fc;
    distinct_prime_factors(phi, factors, &fc);

    for (uint64_t g = 2ULL; g < p; ++g)
    {
        int ok = 1;
        for (size_t i = 0; i < fc; ++i)
        {
            if (pow_mod(g, phi / factors[i], p) == 1ULL)
            {
                ok = 0;
                break;
            }
        }
        if (ok)
            return g;
    }
    return 0ULL; /* should never happen */
}

/* Binary search for value `v` in the sorted array `baby` of size `m`.
 * Returns exponent if found, otherwise (size_t)-1.  Each entry is a pair
 * (value, exponent). */
static size_t
binary_search(const uint64_t *baby_val, const uint64_t *baby_exp,
              size_t m, uint64_t v)
{
    size_t lo = 0, hi = m;
    while (lo < hi)
    {
        size_t mid = (lo + hi) / 2;
        if (baby_val[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < m && baby_val[lo] == v)
        return baby_exp[lo];
    return (size_t)-1;
}

/* Discrete logarithm base `g` of `h` modulo prime `p` (p > 2).
 * Uses baby‑step giant‑step.  Assumes 0 < g, h < p. */
static uint64_t
discrete_log(uint64_t g, uint64_t h, uint64_t p)
{
    uint64_t m = (uint64_t)(ceil(sqrt((double)(p - 1ULL))));
    /* Baby steps */
    uint64_t *baby_val = (uint64_t *)malloc(m * sizeof(uint64_t));
    uint64_t *baby_exp = (uint64_t *)malloc(m * sizeof(uint64_t));
    if (!baby_val || !baby_exp)
    {
        free(baby_val);
        free(baby_exp);
        return 0ULL; /* memory failure → 0 */
    }
    uint64_t cur = 1ULL;
    for (uint64_t i = 0; i < m; ++i)
    {
        baby_val[i] = cur;
        baby_exp[i] = i;
        cur = (cur * g) % p;
    }
    /* sort baby steps by value */
    for (uint64_t i = 1; i < m; ++i)
    {
        uint64_t val = baby_val[i];
        uint64_t exp = baby_exp[i];
        uint64_t j = i;
        while (j > 0 && baby_val[j - 1] > val)
        {
            baby_val[j] = baby_val[j - 1];
            baby_exp[j] = baby_exp[j - 1];
            --j;
        }
        baby_val[j] = val;
        baby_exp[j] = exp;
    }

    /* Pre‑compute g^(-m) mod p */
    uint64_t g_m_inv = pow_mod(g, p - 1ULL - m, p);

    cur = h % p;
    for (uint64_t j = 0; j <= m; ++j)
    {
        size_t exp = binary_search(baby_val, baby_exp, m, cur);
        if (exp != (size_t)-1)
        {
            uint64_t ans = j * m + exp;
            free(baby_val);
            free(baby_exp);
            return ans;
        }
        cur = (cur * g_m_inv) % p;
    }
    free(baby_val);
    free(baby_exp);
    return 0ULL; /* should not happen for a prime modulus */
}

seq_t a(idx_t n)
{
    if (n < 4ULL)
        return 0; /* outside OEIS offset */

    /* 1.  Prime p = n‑th prime (offset 1 → 2) */
    uint64_t p = get_prime(n);          /* returns uint64_t */
    if (p < 7ULL)                       /* n ≥ 4 guarantees p ≥ 7 */
        return 0;

    /* 2.  Smallest primitive root of p */
    uint64_t g = smallest_primitive_root(p);

    /* 3.  Discrete logarithm of 6 base g modulo p */
    uint64_t h = 6ULL % p;
    uint64_t log = discrete_log(g, h, p);

    return (seq_t)log;
}
