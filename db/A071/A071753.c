/*
 * A071753  Expansion of (1+x^4*C^3)*C^4, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1 + x^4*C^3)*C^4 where C = (1 -
 *            sqrt(1-4x))/(2x) is the Catalan generating function.
 *            Equivalently, a(n) = coeff(C^4, n) + coeff(C^7, n-4) for n ≥ 4,
 *            and a(n) = coeff(C^4, n) for n < 4. The coefficient of x^m in
 *            C^k is a_k(m) = k/(2m + k) * binom(2m+k, m)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t kth_coeff(idx_t n, idx_t k)
{
    seq_t bin = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        bin = bin * (seq_t)(n + k + i);
        bin = bin / (seq_t)i;          /* division is exact */
    }
    seq_t term = bin * (seq_t)k;
    term = term / (seq_t)(2 * n + k);  /* division is exact */
    return term;
}

seq_t a(idx_t n)
{
    seq_t res4 = kth_coeff(n, 4);
    seq_t res7 = 0;
    if (n >= 4)
        res7 = kth_coeff(n - 4, 7);
    return res4 + res7;
}
