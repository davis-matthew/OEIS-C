/*
 * A137511  A triangular sequence from coefficients of an expansion of the
 *          Poisson's kernel: p(t,r)=(1-r^2)/(1-2*r*Cos(t)+r^2):
 *          r->t;Cos(t)->x.
 * https://oeis.org/A137511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   A137511 For n ≥ 1 let r,c satisfy n = r*(r+1)/2 + c + 1 (0 ≤ c ≤
 *            r). If r−c is odd, a(n) = 0. Otherwise set k = (r−c)/2, t =
 *            (r+c)/2. Then a(n) = −2 · r! · [ (−1)^k · binom(t,k) · r · 2^c /
 *            (r+c) ] , where a(1) = −1
 * Range:     i8 n <= 12, i16 n <= 25, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   binom, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-1;

    idx_t idx = n - 1;                     /* zero‑based index in triangle */

    /* find row r: r(r+1)/2 ≤ idx < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((seq_t)r * (r + 1) / 2 <= idx) ++r;
    --r;

    idx_t c = idx - (r * (r + 1) / 2);

    /* odd parity → zero */
    if (((r - c) & 1) != 0u)
        return (seq_t)0;

    idx_t k = (r - c) / 2;
    idx_t t = (r + c) / 2;

    seq_t bin = binom(t, k);
    seq_t coeff = bin * (seq_t)r;          /* binom(t,k) * r */
    coeff *= pow2(c);                      /* * 2^c */

    seq_t denom = (seq_t)(2 * t);          /* 2*t = r+c for even parity */
    coeff /= denom;                        /* exact division */

    if (k & 1)                              /* apply (−1)^k */
        coeff = -coeff;

    seq_t fact = factorial(r);              /* r! */

    seq_t result = (seq_t)-2;
    result *= fact;
    result *= coeff;

    return result;
}
