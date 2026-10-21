/*
 * A082072  Smallest prime that divides sigma(n) = A000203(n) and sigma_2(n) =
 *          A001157(n), or 1 if sigma(n) and sigma_2(n) are relatively prime.
 * https://oeis.org/A082072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 288, u16 n <= 8918, u32 n <= 8918, u64 n <= 8918,
 *            u128 n <= 8918, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest prime that divides σ(n) and σ₂(n),
   or 1 if σ(n) and σ₂(n) are coprime.  Here σ(n) is the
   sum of divisors (OEIS A000203) and σ₂(n) is the sum of
   squares of divisors (OEIS A001157).  The function works
   for every n ≥ 1, without assuming increasing input order. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* Euclidean algorithm for 64‑bit unsigned integers. */

/* Returns the smallest prime factor of n (n ≥ 2).  If n itself is
   prime it returns n.  The routine uses trial division, fast enough
   for the sizes of gcd values encountered here (≤ 2⁶⁴). */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if ((n & 1u) == 0u) return 2u;
    for (uint64_t p = 3u; p * p <= n; p += 2u)
    {
        if (n % p == 0u) return p;
    }
    return n;            /* n is prime */
}

seq_t a(idx_t n)
{
    /* Protect against the undefined offset used in OEIS: a(0) is not defined. */
    if (n == 0u) return (seq_t)0u;

    /* Compute σ(n) and σ₂(n) by enumerating divisors up to √n. */
    uint64_t sigma1 = 0u;
    uint64_t sigma2 = 0u;
    for (idx_t i = 1u; i <= n / i; ++i)
    {
        if (n % i == 0u)
        {
            uint64_t d = i;
            uint64_t q = n / i;
            sigma1 += d;
            sigma2 += d * d;
            if (q != d)
            {
                sigma1 += q;
                sigma2 += q * q;
            }
        }
    }

    /* Compute gcd and its smallest prime factor. */
    uint64_t g = gcd64(sigma1, sigma2);
    if (g <= 1u) /* coprime case */
        return (seq_t)1u;

    return (seq_t)smallest_prime_factor(g);
}
