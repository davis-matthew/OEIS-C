/*
 * A121012  Numerators of partial alternating sums of Catalan numbers scaled
 *          by powers of 1/(11^2) = 1/121.
 * https://oeis.org/A121012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of r(n) where r(n) = Σ_{k=0..n} (-1)^k * C(k) /
 *            121^k (C(k) = Catalan number A000108(k)). The fraction r(n) is
 *            reduced to lowest terms. The algorithm keeps the exact numerator
 *            by iteratively summing the series and reducing after each term
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   catalan, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"   // catalan(idx_t k) -> seq_t
#include "../utils/gcd.h"       // gcd(seq_t a, seq_t b) -> seq_t

seq_t a(idx_t n)
{
    /* Starting values for r(0) = 1 = 1/1 */
    seq_t num = 1;          /* numerator of current r */
    seq_t den = 1;          /* denominator of current r  */
    seq_t pow121 = 1;       /* 121^k, will be updated each step */

    /* For k = 1 .. n accumulate terms (-1)^k * C(k) / 121^k  */
    for (idx_t k = 1; k <= n; ++k) {
        /* Update 121^k */
        pow121 *= (seq_t)121;            /* pow121 = 121^k */

        /* The common denominator for r(k‑1) and the new term is 121^k.
           The multiplier needed to bring r(k‑1) to this denominator is
           121^k / den.  Since den | 121^k this division is exact. */
        seq_t multiplier = pow121 / den; /* integer division */

        /* Compute the k‑th Catalan number */
        seq_t cat = catalan(k);

        /* Update numerator:
           r(k) = (num * multiplier) + (-1)^k * cat   (all over denom)
           The result is always positive, so subtraction is safe. */
        if (k & 1U) { /* odd: subtract */
            num = num * multiplier - cat;
        } else {      /* even: add */
            num = num * multiplier + cat;
        }

        /* The new unsimplified denominator is 121^k */
        den = pow121;

        /* Reduce the fraction to lowest terms for the next iteration */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
    }

    /* After the loop num contains the numerator of r(n) in lowest terms. */
    return num;
}
