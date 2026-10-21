/*
 * A071737  Expansion of (1+x^3*C^3)*C^4, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1 + x^3*C^3)*C^4, where C(x) = (1
 *            - sqrt(1-4x))/(2x) is the Catalan generating function.
 *            Equivalently, a(n) = S4(n) + (n>=3 ? S7(n-3) : 0), where S_k(m)
 *            = [x^m] C(x)^k = k/(2m+k) * binomial(2m+k, m)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t S_k(idx_t n, idx_t k)
{
    /* Computes S_k(n) = k/(2n+k) * binomial(2n+k, n) */
    seq_t bin = binomial(2 * n + k, n);
    seq_t num = (seq_t)k * bin;          /* k * binomial      */
    seq_t den = 2 * n + k;               /* 2n + k            */
    return num / den;                   /* exact division    */
}

seq_t a(idx_t n)
{
    seq_t res = S_k(n, 4);               /* coefficient from C^4   */
    if (n >= 3) {
        seq_t extra = S_k(n - 3, 7);     /* coefficient from x^3*C^7 */
        res += extra;
    }
    return res;
}
