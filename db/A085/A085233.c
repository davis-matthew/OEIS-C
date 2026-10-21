/*
 * A085233  Numbers k such that k is divisible by a power of its smallest
 *          prime factor that exceeds its square root.
 * https://oeis.org/A085233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer k such that k is divisible by
 *            a power of its smallest prime factor that exceeds sqrt(k)
 * Range:     u8 n <= 89, u16 n <= 7476, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit value */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0;
    uint64_t high = 0xFFFFFFFFULL + 1;            /* 2^32, > sqrt(ULLONG_MAX) */
    while (low + 1 < high)
    {
        uint64_t mid = low + (high - low) / 2;
        if (mid == 0)
            low = mid;
        else if (mid <= n / mid)                  /* mid*mid <= n without overflow */
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Prime sieve (smallest primes only) */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static uint64_t primes_limit = 1; /* largest prime known */

/* Ensure that all primes ≤ limit are in the array */
static void ensure_primes(uint64_t limit)
{
    if (limit <= primes_limit)
        return;
    for (uint64_t x = primes_limit + 1; x <= limit; ++x)
    {
        bool isprime = true;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > x)
                break;
            if (x % p == 0)
            {
                isprime = false;
                break;
            }
        }
        if (isprime)
        {
            if (primes_cnt == primes_cap)
            {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *newp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!newp)
                    exit(1);                       /* allocation failure */
                primes = newp;
            }
            primes[primes_cnt++] = (uint32_t)x;
        }
    }
    primes_limit = limit;
}

/* Smallest prime factor of n (n ≥ 2) */
static uint64_t spf_uint64(uint64_t n)
{
    if (n < 2)
        return n;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > n)
            break;
        if (n % p == 0)
            return p;
    }
    return n;
}

/* Test whether a number k satisfies the A085233 condition */
static bool accept_k(uint64_t k)
{
    uint64_t sqrtk = isqrt_u64(k);
    uint64_t p = spf_uint64(k);
    uint64_t power = 1;
    uint64_t rem = k;
    while (rem % p == 0)
    {
        power *= p;
        rem /= p;
        if (power > sqrtk)
            return true;
    }
    return false;
}

/* A085233 sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for index 0, harmless */

    idx_t found = 0;
    uint64_t k = 2;                      /* first positive integer candidate */

    while (true)
    {
        uint64_t sqrtk = isqrt_u64(k);
        ensure_primes(sqrtk);
        if (accept_k(k))
        {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
}
