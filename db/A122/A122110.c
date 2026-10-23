/*
 * A122110  Number of integers m of the form (1+2x+4x^3)/(x+n).
 * https://oeis.org/A122110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * d(|1 - 2n - 4n³|) (d(k) = number of positive divisors
 *            of k)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Utility to build primes up to a limit */
#include "../utils/build_primes_upto.h"

/* Integer square root for 64‑bit unsigned values */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (1ULL << 32) - 1;
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Global prime cache */
static uint64_t *prime_list  = NULL;
static size_t   num_primes   = 0;
static idx_t    current_lim  = 0;

/* Ensure that all primes ≤ limit are available in prime_list */
static void ensure_primes_limit(idx_t limit)
{
    if (limit <= current_lim) return;

    /* Rebuild the prime list if necessary */
    if (prime_list) free(prime_list);
    build_primes_upto(limit, &prime_list, &num_primes);
    current_lim = limit;
}

/* Count positive divisors of a 64‑bit unsigned integer */
static seq_t divisor_count_u64(uint64_t n)
{
    seq_t d = 1;
    size_t i = 0;
    for (; i < num_primes; ++i)
    {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        uint64_t exp = 0;
        while (n % p == 0)
        {
            n /= p;
            ++exp;
        }
        if (exp) d *= (seq_t)(exp + 1);
    }
    if (n > 1)          /* remaining prime factor > sqrt(original n) */
        d *= (seq_t)2;
    return d;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute D = 1 - 2n - 4n^3 */
    int64_t nn   = (int64_t)n;
    int64_t D    = 1 - 2 * nn - 4 * nn * nn * nn;

    /* Use absolute value of D */
    uint64_t absD = (D < 0) ? (uint64_t)(-D) : (uint64_t)D;

    /* Prepare primes up to sqrt(absD) */
    uint64_t sqrt_lim = isqrt_u64(absD);
    ensure_primes_limit((idx_t)sqrt_lim);

    /* Number of positive divisors of |D| */
    seq_t pos_div = divisor_count_u64(absD);

    /* Each positive divisor gives two integers m (for ±d) */
    return (seq_t)(pos_div * 2);
}
