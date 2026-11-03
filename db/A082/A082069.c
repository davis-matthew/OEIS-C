/*
 * A082069  Smallest common prime-divisor of n and Sigma_2(n) = A001157(n);
 *          a(n) = 1 if no common prime-divisor exists.
 * https://oeis.org/A082069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest common prime divisor of n and σ₂(n) (σ₂(n)
 *            is the sum of squares of the divisors of n). If n and σ₂(n) are
 *            coprime the value is 1. The implementation uses the provided
 *            functions `sigma_k`, `gcd` and `floor_sqrt`
 * Range:     u8 n <= 14, u16 n <= 1994, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9279 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

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

/* Find the smallest prime factor of a positive `seq_t` value `x`.  For
   small values the function falls back to trial division.  For huge
   values a simple loop up to `floor_sqrt(x)` is used - adequate for
   the problem limits. */
static seq_t smallest_prime_factor(seq_t x)
{
    /* If x is 0 or 1, no prime factor exists; return 1 */
    if (x <= 1) return 1;

    /* Try casting to 64‑bit when safe */
    if (x <= UINT64_MAX) {
        uint64_t v = (uint64_t)x;
        for (uint64_t p = 2; p * p <= v; ++p)
            if (v % p == 0)
                return (seq_t)p;
        return (seq_t)v;               /* x itself is prime */
    }

    /* General case: trial division up to floor_sqrt(x) */
    seq_t limit = floor_sqrt(x);
    uint64_t lim = (uint64_t)limit;    /* limit will be small enough for the loop for our n */
    for (uint64_t p = 2; p <= lim; ++p)
        if (x % (seq_t)p == 0)
            return (seq_t)p;
    return x;                            /* x is prime */
}

seq_t a(idx_t n)
{
    /* Offset is 1: the first term corresponds to n = 1 */
    /* Compute σ₂(n) = sum of squares of divisors */
    seq_t sigma = sigma_k(n, 2);

    /* Compute common divisor g = gcd(n, σ₂(n)) */
    seq_t g = gcd((seq_t)n, sigma);

    /* If coprime, return 1; otherwise the smallest prime factor of g */
    if (g == 1)
        return 1;

    return smallest_prime_factor(g);
}
