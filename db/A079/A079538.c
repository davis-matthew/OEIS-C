/*
 * A079538  a(n) = sigma[k](n) - phi(n)^k - d(n)^k for k=2.
 * https://oeis.org/A079538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma_k(n,2) - phi(n)^2 - d(n)^2 where sigma_k(n,2) is
 *            the sum of squares of the divisors of n, phi(n) is Euler's
 *            totient and d(n) the number of divisors
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 2458, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   seq_pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow_uint.h"

/* The build supplies the integer types seq_t (signed) and idx_t (unsigned). */

/* Integer square root of an unsigned 64‑bit value (floor).  Uses a
   binary search and __uint128_t to avoid overflow.  The result fits
   in idx_t. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* Compute the OEIS A079538 term for index n (n >= 1).  For n < 1
   the function returns 0; the sequence is defined from offset 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n == 0 */

    /* ---------- factorisation ---------- */
    idx_t m = n;
    seq_t phi   = n;       /* initialise φ(n) = n */
    seq_t tau   = 1;       /* initialise d(n) = 1 */
    seq_t sigma = 1;       /* initialise σ_2(n) = 1 */

    /* trial division up to sqrt(m) */
    for (idx_t p = 2; (__uint128_t)p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }

            /* update φ(n):  φ *= (1 – 1/p) */
            phi -= phi / p;

            /* update d(n):  d *= (exp + 1) */
            tau *= (exp + 1);

            /* update σ_2(n) using the formula for a prime power p^exp
               σ_2(p^exp) = (p^{2(exp+1)} – 1) / (p^2 – 1)  */
            seq_t p2      = seq_pow_uint((seq_t)p, 2);          /* p^2   */
            seq_t num     = seq_pow_uint((seq_t)p, 2 * (exp + 1)) - 1; /* p^{2(exp+1)} – 1 */
            seq_t den     = p2 - 1;     /* p^2 – 1 */
            sigma *= num / den;
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1) {
        /* new prime factor p = m with exponent 1 */
        idx_t p = m;
        phi -= phi / p;
        tau *= 2;               /* (1 + 1) */

        seq_t p2      = (seq_t)p * (seq_t)p;
        seq_t num     = p2 * p2 - 1;  /* p^{2(1+1)} – 1 = p^4 – 1 */
        seq_t den     = p2 - 1;        /* p^2 – 1 */
        sigma *= num / den;
    }

    seq_t phi_sq = phi * phi;
    seq_t tau_sq = tau * tau;
    return sigma - phi_sq - tau_sq;
}
