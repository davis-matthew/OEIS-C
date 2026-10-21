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
 * Range:     u8 n <= 14, u16 n <= 1994, u32 n <= 9279, u64 n <= 9279,
 *            u128 n <= 9279, big any n
 * Helpers:   floor_sqrt, gcd, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/gcd.h"
#include "../utils/sigma_k.h"
#include "../utils/floor_sqrt.h"

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
