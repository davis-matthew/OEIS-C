/*
 * A121920  a(n) = n-th integer from among those positive integers k where
 *          GCD(k,n) = prime power >1. a(1) = 1.
 * https://oeis.org/A121920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A121920: a(n) = n-th positive integer k such that
   gcd(k,n) is a prime power > 1. The value is found by binary
   searching the k that reaches the n-th count of such integers.
   The counting is performed using inclusion–exclusion over the
   distinct prime factors of n. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* helper: count integers <= K whose gcd with n is a prime power >1
   primes[0..m-1] are the distinct prime factors of n */
static idx_t
count_upto(__uint128_t K, const uint64_t *pr, size_t m)
{
    if (m == 0)
        return 0;
    __uint128_t res = 0;
    uint64_t masks = ((uint64_t)1 << m) - 1;
    for (uint64_t mask = 1; mask <= masks; ++mask) {
        __uint128_t prod = 1;
        size_t sz = 0;
        for (size_t i = 0; i < m; ++i) {
            if (mask & ((uint64_t)1 << i)) {
                prod *= (__uint128_t)pr[i];
                ++sz;
            }
        }
        uint64_t cnt = (uint64_t)(K / prod);
        if (sz & 1)
            res += (__uint128_t)cnt * sz;
        else
            res -= (__uint128_t)cnt * sz;
    }
    return (idx_t)res;
}

seq_t
a(idx_t n)
{
    if (n == 1)
        return 1;               /* a(1) is defined as 1 */

    uint64_t n64 = (uint64_t)n;     /* idx_t is uint64_t */
    uint64_t primes[64], exps[64];
    size_t m;
    factor_u64(n64, primes, exps, &m);
    /* m is the number of distinct prime factors of n */

    /* binary search for the smallest k such that count_upto(k) >= n */
    __uint128_t low = 1;
    __uint128_t high = (__uint128_t)n64 * n64;  /* n² is an upper bound */

    /* ensure high is large enough */
    while (count_upto(high, primes, m) < n) {
        high <<= 1;
    }

    __uint128_t ans = high;
    while (low <= high) {
        __uint128_t mid = (low + high) >> 1;
        idx_t c = count_upto(mid, primes, m);
        if (c >= n) {
            ans = mid;
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return (seq_t)ans;
}
