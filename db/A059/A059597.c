/*
 * A059597  Ninth column (m=8) of convolution triangle A059594(n,m).
 * https://oeis.org/A059597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A059597 – Ninth column (m=8) of convolution triangle A059594.
 *
 * Formula:
 *    a(n) = binomial( floor(n/2)+13 , 13 ) *
 *           P( floor(n/2) ) / 1785
 *
 *    where P_even(k) = 8k^4 + 190k^3 + 1414k^2 + 3488k + 1785
 *          P_odd (k) = 8k^4 + 258k^3 + 2842k^2 + 12192k + 16065
 *
 * The division by 1785 is exact for all integer n.
 *
 * The implementation uses only integer operations compatible with all
 * integer types supported by the build (uint8_t … uint64_t, unsigned
 * __int128, or arbitrary‑precision).  A local gcd routine is used to
 * avoid overflow on 64‑bit builds.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* Compute the even‑index polynomial in k. */
static seq_t poly_even(idx_t k)
{
    seq_t kk = (seq_t)k;
    seq_t kk2 = kk * kk;          /* k^2 */
    seq_t kk3 = kk2 * kk;          /* k^3 */
    seq_t kk4 = kk3 * kk;          /* k^4 */
    return 8   * kk4
         + 190 * kk3
         + 1414 * kk2
         + 3488 * kk
         + 1785;
}

/* Compute the odd‑index polynomial in k. */
static seq_t poly_odd(idx_t k)
{
    seq_t kk = (seq_t)k;
    seq_t kk2 = kk * kk;          /* k^2 */
    seq_t kk3 = kk2 * kk;          /* k^3 */
    seq_t kk4 = kk3 * kk;          /* k^4 */
    return 8   * kk4
         + 258 * kk3
         + 2842 * kk2
         + 12192 * kk
         + 16065;
}

/* Euclidean algorithm for seq_t values. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    const seq_t DENOM_INIT = (seq_t)1785;

    idx_t k = n >> 1;                     /* floor(n/2) */
    seq_t binom = binomial(k + 13, 13);   /* binomial(k+13,13) */
    seq_t poly = (n & 1) ? poly_odd(k) : poly_even(k);

    /* Reduce the factor 1785 from binom and poly to prevent overflow. */
    seq_t denom = DENOM_INIT;

    /* First reduce binom. */
    seq_t g = seq_gcd(binom, denom);
    if (g != (seq_t)1) {
        binom /= g;
        denom /= g;
    }

    /* Then reduce poly. */
    g = seq_gcd(poly, denom);
    if (g != (seq_t)1) {
        poly /= g;
        denom /= g;
    }

    /* Now denom should be 1, but keep division just in case. */
    seq_t result = binom * poly;
    if (denom != (seq_t)1)
        result /= denom;

    return result;
}
