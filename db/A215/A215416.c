/*
 * A215416  Numerator of zeta function multiple arising in Zig-Zag Conjecture.
 * https://oeis.org/A215416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*((2*n-2)!)/(n!*(n-1)!)*(1 - ((-1)^n)/(2^(2*n-3)))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special‑case n = 1 (the formula involves 2^(2n‑3), which is undefined for n = 1). */
    if (n == 1) return (seq_t)12;

    /* C = binomial(2*n-2, n-1) = (2n-2)!/((n-1)!*(n-1)!) */
    idx_t a2 = 2 * n - 2;
    idx_t b2 = n - 1;
    seq_t C = binomial(a2, b2);

    /* v2(C) – number of trailing zero bits in C */
    seq_t C_tmp = C;
    idx_t v2C = 0;
    while ((C_tmp & (seq_t)1) == (seq_t)0) {
        C_tmp >>= 1;
        v2C++;
    }

    /* v2(n) – number of trailing zero bits in n */
    idx_t n_tmp = n;
    idx_t v2n = 0;
    while ((n_tmp & 1ULL) == 0ULL) {
        n_tmp >>= 1;
        v2n++;
    }

    /* B = 4 * C / n  (exact division) */
    seq_t C_div_n = C / (seq_t)n;
    seq_t B = C_div_n * (seq_t)4;

    /* v2(B) = v2(C) + 2 – v2(n)  (since B = 4*C/n) */
    idx_t v2B = (v2C + 2) - v2n;

    /* t = min(v2(B), 2*n-3)  – powers of two to cancel */
    idx_t denom_exp = 2 * n - 3;
    idx_t t = v2B;
    if (t > denom_exp) t = denom_exp;

    /* B_div = B / 2^t */
    seq_t B_div = B >> t;

    /* Power of two part of the fraction */
    seq_t pow2exp = pow2(denom_exp);

    /* S = 2^(2n-3) + (-1)^(n+1)  = (odd) ; compute it as ±1 */
    seq_t S;
    if ((n & 1) == 0)      /* n even: (-1)^(n+1) = -1 */
        S = pow2exp - (seq_t)1;
    else                   /* n odd : (-1)^(n+1) =  1 */
        S = pow2exp + (seq_t)1;

    /* Final result: a(n) = (B/2^t) * S  */
    return B_div * S;
}
