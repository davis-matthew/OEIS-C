/*
 * A071744  Expansion of (1+x^4*C)*C^3, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ((1 + x^4 * C(x)) * C(x)^3) * where C(x) = (1 -
 *            sqrt(1-4x)) / (2x) is the generating function * for the Catalan
 *            numbers. In other words, * a(n) = coeff(C^3, n) + coeff(C^4,
 *            n-4) (with the second term zero for n<4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* coefficient of x^n in C(x)^p :  p/(2n+p) * binom(2n+p, n) */
static seq_t coeff_pow_p(idx_t p, idx_t n)
{
    idx_t N = 2 * n + p;                       /* total for binomial */
    seq_t comb = binom(N, n);                  /* binom(2n+p, n)   */
    seq_t num  = (seq_t)p * comb;              /* p * comb         */
    seq_t den  = (seq_t)N;                     /* 2n + p           */
    return num / den;                          /* integer division */
}

seq_t a(idx_t n)
{
    seq_t result = coeff_pow_p(3, n);          /* coefficient of C^3   */
    if (n >= 4) {
        result += coeff_pow_p(4, n - 4);       /* + coefficient of C^4 at n-4 */
    }
    return result;
}
