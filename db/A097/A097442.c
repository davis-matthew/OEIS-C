/*
 * A097442  Integer part of the area of consecutive prime sided isosceles
 *          triangles.
 * https://oeis.org/A097442
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( p * sqrt(4*q*q - p*p) / 4 ) where p = prime_{n-1}
 *            and q = prime_n (primes are 2‑based: a(2) uses 2,3).
 *            Implementation uses integer arithmetic only
 * Range:     u8 n <= 9, u16 n <= 77, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Utils: build primes up to a limit */
#include "../utils/build_primes_upto.h"

/* Persistent storage for primes */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* Estimate an upper bound for the nth prime (n >= 6). */
static uint64_t estimate_prime_limit(idx_t n)
{
    if (n < 6) return 15;
    double nn = (double)n;
    double est = nn * (log(nn) + log(log(nn))) + 10.0;
    return (uint64_t)est;
}

/* Ensure at least n primes are stored in the global array. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    uint64_t limit = estimate_prime_limit(n);
    while (true) {
        if (primes) free(primes);
        primes = NULL;
        prime_cnt = 0;
        build_primes_upto(limit, &primes, &prime_cnt);
        if (prime_cnt >= (size_t)n) break;
        limit *= 2;   /* increase the limit */
    }
}

/* Integer square root for unsigned __int128 using binary search.
   The result fits in 64 bits for the required range. */
static unsigned long long isqrt_uint128(unsigned __int128 x)
{
    unsigned long long low = 0;
    unsigned long long high = (unsigned long long)1 << 35;  /* > sqrt(4·prime^4) for primes < 110k */
    while (low < high) {
        unsigned long long mid = (low + high + 1) >> 1;
        unsigned __int128 midsq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (midsq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;   /* floor(sqrt(x)) */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* defined for n >= 2 only */

    /* Ensure the required primes are available */
    ensure_primes((idx_t)n);

    /* Retrieve the consecutive primes */
    uint64_t p = primes[n - 2];   /* (n-1)-th prime */
    uint64_t q = primes[n - 1];   /* n-th prime   */

    /* Compute D = 4*q^2 - p^2   (fits in 64 bits) */
    uint64_t D = 4ULL * q * q - p * p;

    /* Compute T = p^2 * D   (fits in 128 bits) */
    unsigned __int128 T = (unsigned __int128)p * (unsigned __int128)p * (unsigned __int128)D;

    /* sqrt_T = floor(sqrt(T)) */
    unsigned long long sqrt_T = isqrt_uint128(T);

    /* Final answer = floor(sqrt_T / 4) */
    return (seq_t)(sqrt_T / 4);
}
