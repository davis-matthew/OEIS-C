/*
 * A071729  Expansion of (1+x^3*C)*C^4, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ((1+x^3*C(x))*C(x)^4) = a4(n) + (n>=3 ? a5(n-3) :
 *            0) where a4(k) = 4/(2k+4) * binom(2k+4, k) a5(k) = 5/(2k+5) *
 *            binom(2k+5, k) C(x) is the Catalan generating function: C(x) = Σ
 *            cat(k)x^k
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a4(n) */
    idx_t denom4 = 2 * n + 4;
    seq_t coeff = binom(denom4, n);          /* binom(2n+4, n) */
    coeff = coeff * (seq_t)4;                /* multiply by 4   */
    coeff = coeff / (seq_t)denom4;           /* divide by 2n+4   */

    /* add a5(n-3) if n>=3 */
    if (n >= 3) {
        idx_t m = n - 3;
        idx_t denom5 = 2 * m + 5;
        seq_t term5 = binom(denom5, m);      /* binom(2m+5, m)  */
        term5 = term5 * (seq_t)5;            /* multiply by 5    */
        term5 = term5 / (seq_t)denom5;       /* divide by 2m+5   */
        coeff += term5;
    }
    return coeff;
}
