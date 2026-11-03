/*
 * A081382  a(1) = 1, for n > 1 a(n) = Min{x > n, A008472(x) = A008472(n)}.
 * https://oeis.org/A081382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{x > n : A008472(x) = A008472(n)}
 * Range:     u8 n <= 10, u16 n <= 910, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8868 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A008472(n) = sum of distinct prime factors of n. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit_u64 = 0;

/* Ensure prime_list contains all primes <= limit (in uint64_t). */
static void ensure_primes_u64(idx_t limit)
{
    if (prime_limit_u64 >= limit)
        return;
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit_u64 = limit;
}

/* Sum of distinct prime factors of x (x fits in uint64_t). */
static uint64_t sum_distinct_prime_factors_u64(uint64_t x)
{
    if (x < 2)
        return 0;
    idx_t lim = (idx_t)floor_sqrt((seq_t)x) + 1;
    ensure_primes_u64(lim);
    uint64_t sum = 0;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= x; ++i) {
        uint64_t p = prime_list[i];
        if (x % p == 0) {
            sum += p;
            while (x % p == 0)
                x /= p;
        }
    }
    if (x > 1) /* remaining prime factor > sqrt(x) */
        sum += x;
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;  /* defined a(1)=1 */
    uint64_t target = sum_distinct_prime_factors_u64((uint64_t)n);
    for (uint64_t m = (uint64_t)n + 1;; ++m) {
        if (sum_distinct_prime_factors_u64(m) == target)
            return (seq_t)m;
    }
}
