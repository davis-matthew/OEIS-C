/*
 * A112327  Triangle read by rows: T(n,k)=k^3*2^k*binomial(2n-k,n-k)/(2n-k)
 *          (1<=k<=n).
 * https://oeis.org/A112327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) equals the n‑th element of the triangle
 *            T(n,k)=k^3*2^k*binomial(2n-k,n-k)/(2n-k) read by rows
 * Range:     u8 n <= 9, u16 n <= 38, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, gcd_any, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/gcd_any.h"

seq_t a(idx_t n)
{
    /* Sequence is 1‑based; term 0 is undefined – return 0. */
    if (n == 0)
        return 0;

    /* Find the row r such that the cumulative number of terms up to r
       is at least n, i.e. r(r+1)/2 ≥ n. */
    idx_t r = 1;
    idx_t cum = 1;               /* 1 + 2 + … + r */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Column index k within row r (1‑based). */
    idx_t k = n - (r - 1) * r / 2;

    /* Convert to seq_t for the arithmetic below. */
    seq_t seq_r = (seq_t)r;
    seq_t seq_k = (seq_t)k;

    /* Binomial(2r-k, r-k). */
    seq_t B = binomial(2 * r - k, r - k);
    seq_t denom = (seq_t)(2 * r - k);   /* 2n‑k */

    /* k^3 and 2^k. */
    seq_t k3 = seq_k * seq_k * seq_k;
    seq_t two_pow = pow2(k);            /* 2^k */

    /* Reduce the denominator by common factors with the multipliers
       to keep intermediate values within bounds. */
    seq_t g = gcd_any(k3, denom);
    if (g != (seq_t)1u) {
        k3 /= g;
        denom /= g;
    }

    g = gcd_any(two_pow, denom);
    if (g != (seq_t)1u) {
        two_pow /= g;
        denom /= g;
    }

    g = gcd_any(B, denom);
    if (g != (seq_t)1u) {
        B /= g;
        denom /= g;
    }

    /* After removing all common factors, denom should be 1.
       Multiply the remaining factors to obtain the final value. */
    seq_t val = B;
    val *= k3;
    val *= two_pow;

    /* If a small residue of denom remains, divide it now
       (this should normally not happen for valid inputs). */
    if (denom != (seq_t)1u)
        val /= denom;

    return val;
}
