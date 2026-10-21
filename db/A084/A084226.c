/*
 * A084226  Denominators of successive approximations to zeta(3) = Sum_{k>0}
 *          1/k^3, using Zeilberger's formula with s=3.
 * https://oeis.org/A084226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the n‑th partial sum of Zeilberger’s *
 *            rational approximation of ζ(3). The partial sum is * * S(n) =
 *            Σ_{k=0..n} (1/72)(-1)^k * ·
 *            (5265k^4+13878k^3+13761k^2+6120k+1040) / * [ binom(3k,k) ·
 *            binom(4k,k) · (4k+1)(4k+3) · * (k+1) · (3k+1)^2 · (3k+2)^2 ]. *
 *            * a(n) is the denominat...
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum_num = 0;
    seq_t sum_den = 1;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Polynomial part of the numerator (always positive). */
        seq_t kt   = (seq_t)k;
        seq_t k2   = kt * kt;
        seq_t k3   = k2 * kt;
        seq_t k4   = k3 * kt;

        seq_t term_abs = (seq_t)5265 * k4
                       + (seq_t)13878 * k3
                       + (seq_t)13761 * k2
                       + (seq_t)6120  * kt
                       + (seq_t)1040;

        /* Denominator of the k‑th term. */
        seq_t term_den = 72;
        term_den *= binomial((idx_t)(3 * k), k);
        term_den *= binomial((idx_t)(4 * k), k);
        term_den *= (seq_t)(4 * k + 1);
        term_den *= (seq_t)(4 * k + 3);
        term_den *= (seq_t)(k + 1);
        term_den *= (seq_t)(3 * k + 1);
        term_den *= (seq_t)(3 * k + 1);
        term_den *= (seq_t)(3 * k + 2);
        term_den *= (seq_t)(3 * k + 2);

        /* Reduce the individual fraction before adding. */
        seq_t g_term = gcd(term_abs, term_den);
        if (g_term > 1) {
            term_abs /= g_term;
            term_den /= g_term;
        }

        /* Combine with the running sum. */
        seq_t g = gcd(sum_den, term_den);
        seq_t sum_den_div_g  = sum_den / g;
        seq_t term_den_div_g = term_den / g;

        seq_t new_den = sum_den_div_g * term_den;   /* lcm of denominators   */
        seq_t comp1   = sum_num * term_den_div_g;   /* contribution from sum */
        seq_t comp2   = term_abs * sum_den_div_g;   /* contribution from term*/

        seq_t new_num;
        if ((k & 1ULL) == 0)          /* even k – positive term   */
            new_num = comp1 + comp2;
        else                          /* odd k – negative term    */
            new_num = comp1 - comp2;

        /* Reduce the new fraction. */
        seq_t g2 = gcd(new_num, new_den);
        if (g2 > 1) {
            new_num /= g2;
            new_den /= g2;
        }

        sum_num = new_num;
        sum_den = new_den;
    }

    return sum_den;
}
