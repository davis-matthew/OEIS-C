/*
 * A066800  Number of different eventual period lengths for power sequences
 *          mod n; i.e., number of different period lengths of repeating
 *          digits of 1/n in different bases.
 * https://oeis.org/A066800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A066800(n) = A000005(A002322(n)) (= number of divisors of
 *            Carmichael λ(n))
 * Range:     u8 n <= 2709, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

/* cache of primes up to a given limit */
static idx_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* ensure prime_cache contains all primes <= limit */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_cache)
    {
        free(prime_cache);
        prime_cache = NULL;
    }
    build_primes_upto(limit, &prime_cache, &prime_cnt);
    prime_limit = limit;
}

/* floor integer square root of a 64‑bit integer (no floating point) */
static inline idx_t int_sqrt(idx_t n)
{
    if (n < 2)
        return n;
    idx_t low = 1, high = n >> 1;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* compute Carmichael λ(n) by factoring n */
static seq_t compute_lambda(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    idx_t temp = n;
    seq_t lambda_val = (seq_t)1;
    for (size_t i = 0; i < prime_cnt && prime_cache[i] * prime_cache[i] <= temp; ++i)
    {
        idx_t p = prime_cache[i];
        if (temp % p == 0)
        {
            size_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            seq_t lam;
            if (p == (idx_t)2)
            {
                if (e == 1)
                    lam = (seq_t)1;
                else if (e == 2)
                    lam = (seq_t)2;
                else
                    lam = ((seq_t)1) << (e - 2);
            }
            else
            {
                seq_t pk = (seq_t)1;
                for (size_t j = 1; j < e; ++j)
                    pk *= (seq_t)p;
                lam = pk * ((seq_t)p - (seq_t)1);
            }
            seq_t g = gcd(lambda_val, lam);
            lambda_val = (lambda_val / g) * lam;
        }
    }
    if (temp > 1)
    {
        idx_t p = temp;
        seq_t lam = (p == (idx_t)2) ? (seq_t)1 : ((seq_t)p - (seq_t)1);
        seq_t g = gcd(lambda_val, lam);
        lambda_val = (lambda_val / g) * lam;
    }
    return lambda_val;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* unused, offset starts at 1 */

    /* factor n -> λ(n) */
    idx_t sqrt_n = int_sqrt(n);
    ensure_primes_upto(sqrt_n);
    seq_t lam_val = compute_lambda(n);

    /* divisor count of λ(n) */
    uint64_t lam = (uint64_t)lam_val;          /* λ(n) fits in 64 bits */
    idx_t sqrt_lam = int_sqrt((idx_t)lam);
    ensure_primes_upto(sqrt_lam);

    uint64_t temp = lam;
    uint64_t divcnt = 1;
    for (size_t i = 0; i < prime_cnt && prime_cache[i] * prime_cache[i] <= temp; ++i)
    {
        uint64_t p = prime_cache[i];
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            divcnt *= (e + 1);
        }
    }
    if (temp > 1)
        divcnt *= 2;   /* remaining prime factor */

    return (seq_t)divcnt;
}
