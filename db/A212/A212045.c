/*
 * A212045  Numerators in the resistance triangle: T(k,n)=b, where b/c is the
 *          resistance distance R(k,n) for k resistors in an n-dimensional
 *          cube.
 * https://oeis.org/A212045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabl
 * Formula:   Uses the recurrence for the resistance distance R(k,n) in an
 *            n‑dimensional unit resistor cube. R(k,n) = ( n·R(k‑1,n) –
 *            (k‑1)·R(k‑2,n) – 2^(1‑n) ) / ( n+1‑k ), with base values
 *            R(0,n)=0 and R(1,n) = (2^n – 1) / ( n·2^(n‑1) ). The entry
 *            T(k,n)=numerator of the reduced fraction R(k,n). The sequence...
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t idx)
{
    /* compute triangle coordinates (k, n) from the linear index */
    double d = (sqrt(8.0 * idx + 1.0) - 1.0) / 2.0;
    idx_t n = (idx_t)(d);
    if (n * (n + 1) / 2 < idx) ++n;
    idx_t k = idx - n * (n - 1) / 2;

    /* special case n=1 */
    if (n == 1 && k == 1) return (seq_t)1;

    /* pre‑compute powers of two: 2^(n-1) and 2^n */
    seq_t pow2 = seq_pow((seq_t)2, n - 1);      /* 2^(n-1) */
    seq_t pow2_n = seq_mul(pow2, (seq_t)2);     /* 2^n */

    /* base fractions for k = 0 and k = 1 */
    seq_t num_prev2 = (seq_t)0;   /* R(0,n) = 0/1 */
    seq_t den_prev2 = (seq_t)1;

    seq_t num_prev1 = pow2_n - (seq_t)1;               /* numerator of R(1,n) */
    seq_t den_prev1 = seq_mul((seq_t)n, pow2);         /* denominator of R(1,n) */
    seq_t g = seq_gcd(num_prev1, den_prev1);
    num_prev1 /= g;
    den_prev1 /= g;

    if (k == 1) return num_prev1;

    /* iterate k = 2 … target k */
    for (idx_t cur = 2; cur <= k; ++cur)
    {
        /* term1 = n · R(cur-1,n) */
        seq_t term1_num = seq_mul((seq_t)n, num_prev1);
        seq_t term1_den = den_prev1;

        /* term2 = (cur-1) · R(cur-2,n) */
        seq_t term2_num = seq_mul((seq_t)(cur - 1), num_prev2);
        seq_t term2_den = den_prev2;

        /* diff1 = term1 – term2 */
        seq_t tmp1 = seq_mul(term1_num, term2_den);
        seq_t tmp2 = seq_mul(term2_num, term1_den);
        seq_t diff1_num = tmp1 - tmp2;          /* term1 ≥ term2 by theory */
        seq_t diff1_den = seq_mul(term1_den, term2_den);
        g = seq_gcd(diff1_num, diff1_den);
        diff1_num /= g;
        diff1_den /= g;

        /* subtract 2^(1-n) = 1 / pow2 */
        seq_t expr_num = seq_mul(diff1_num, pow2) - seq_mul((seq_t)1, diff1_den);
        seq_t expr_den = seq_mul(diff1_den, pow2);
        g = seq_gcd(expr_num, expr_den);
        expr_num /= g;
        expr_den /= g;

        /* divide by (n+1 - cur) */
        seq_t fac = (seq_t)(n + 1 - cur);        /* positive integer */
        seq_t final_num = expr_num;
        seq_t final_den = seq_mul(expr_den, fac);
        g = seq_gcd(final_num, final_den);
        final_num /= g;
        final_den /= g;

        /* shift for next iteration */
        num_prev2 = num_prev1;  den_prev2 = den_prev1;
        num_prev1 = final_num;  den_prev1 = final_den;
    }

    return num_prev1;
}
