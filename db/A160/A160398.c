/*
 * A160398  Numerator of Hermite(n, 17/32).
 * https://oeis.org/A160398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of the physicists' Hermite polynomial H_n(x)
 *            evaluated at x = 17/32 (with H_0 = 1, H_1 = 2x)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;          /* H_0 = 1 */
    if (n == 1) return (seq_t)17;         /* H_1 = 17/16 -> numerator 17 */

    /* Constants in the recurrence */
    const seq_t P = (seq_t)17;            /* 17 */
    const seq_t Q = (seq_t)32;            /* denominator of x */
    const seq_t TWO_P = (seq_t)34;        /* 2 * P */

    /* State for H_{k-1} and H_k */
    seq_t a0_num = (seq_t)1;              /* numerator of H_{k-1} (H_0) */
    seq_t a0_den = (seq_t)1;              /* denominator of H_{k-1} */

    seq_t a1_num = TWO_P;                 /* numerator of H_k (initially H_1) */
    seq_t a1_den = Q;                     /* denominator of H_k */

    /* Reduce H_1 to lowest terms */
    seq_t g = seq_gcd(a1_num < (seq_t)0 ? -a1_num : a1_num, a1_den);
    if (g != (seq_t)1) { a1_num /= g; a1_den /= g; }
    if (a1_den < (seq_t)0) { a1_den = -a1_den; a1_num = -a1_num; }

    /* Iterate from k = 2 to n */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t k_minus_one = (seq_t)(k - 1);       /* (k‑1) as seq_t */

        /* term1 = 2*P * a1_num / (Q * a1_den)   (i.e. 2x * H_k) */
        seq_t t1_num = (seq_t)2 * P * a1_num;
        seq_t t1_den = Q * a1_den;

        /* term2 = 2*(k‑1) * a0_num / a0_den       (i.e. 2*(k‑1)*H_{k‑1}) */
        seq_t t2_num = (seq_t)2 * k_minus_one * a0_num;
        seq_t t2_den = a0_den;

        /* Compute (term1 - term2) with least common denominator. */
        seq_t g_common = seq_gcd(t1_den, t2_den);
        seq_t d1_div = t1_den / g_common;   /* factor to bring term1 to common denom */
        seq_t d2_div = t2_den / g_common;   /* factor to bring term2 to common denom */

        seq_t num = t1_num * d2_div - t2_num * d1_div;
        seq_t den = d1_div * t2_den;        /* the least common denominator */

        /* Reduce the resulting fraction */
        seq_t abs_num = num < (seq_t)0 ? -num : num;
        seq_t g2 = seq_gcd(abs_num, den);
        if (g2 != (seq_t)1) { num /= g2; den /= g2; }
        if (den < (seq_t)0) { den = -den; num = -num; }

        /* Shift states for next iteration */
        a0_num = a1_num;
        a0_den = a1_den;
        a1_num = num;
        a1_den = den;
    }

    /* Return the numerator of H_n(17/32) */
    return a1_num;
}
