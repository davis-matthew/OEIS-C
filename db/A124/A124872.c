/*
 * A124872  Denominator of imaginary part of (3*i - 1)^(-n).
 * https://oeis.org/A124872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 5^n * 2^(n - min(v2, n))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v2 is the exponent of 2 in the imaginary part of (-1-3i)^n */

#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Representation of the complex number (-1-3i)^k */
    seq_t r_abs = (seq_t)1;   /* |Re|        */
    bool   r_neg = false;     /* sign of Re (false = +) */
    seq_t v_abs = (seq_t)0;   /* |Im|        */
    bool   v_neg = false;     /* sign of Im (false = +) */

    for (idx_t k = 0; k < n; ++k) {
        /* save old values for use in both updates */
        seq_t  old_r_abs = r_abs;
        bool   old_r_neg = r_neg;
        seq_t  old_v_abs = v_abs;
        bool   old_v_neg = v_neg;

        /* --- compute new real part r' = -r + 3v --- */
        /* term1 = -r */
        bool   sign1 = !old_r_neg;
        seq_t  abs1   = old_r_abs;

        /* term2 = 3*v */
        seq_t  abs2   = old_v_abs * (seq_t)3;
        bool   sign2  = old_v_neg;

        /* add signed numbers */
        seq_t  new_r_abs;
        bool   new_r_neg;
        if (sign1 == sign2) {
            new_r_abs = abs1 + abs2;
            new_r_neg = sign1;
        } else {
            if (abs1 > abs2) {
                new_r_abs = abs1 - abs2;
                new_r_neg = sign1;
            } else {
                new_r_abs = abs2 - abs1;
                new_r_neg = sign2;
            }
        }

        /* --- compute new imaginary part v' = -(3r + v) --- */
        /* termA = 3*r */
        seq_t  absA   = old_r_abs * (seq_t)3;
        bool   signA  = old_r_neg;

        /* termB = v */
        seq_t  absB   = old_v_abs;
        bool   signB  = old_v_neg;

        /* sum of termA and termB */
        seq_t  sum_abs;
        bool   sum_neg;
        if (signA == signB) {
            sum_abs = absA + absB;
            sum_neg = signA;
        } else {
            if (absA > absB) {
                sum_abs = absA - absB;
                sum_neg = signA;
            } else {
                sum_abs = absB - absA;
                sum_neg = signB;
            }
        }

        /* v' = -sum */
        seq_t  new_v_abs = sum_abs;
        bool   new_v_neg = !sum_neg;

        /* Update for next iteration */
        r_abs = new_r_abs; r_neg = new_r_neg;
        v_abs = new_v_abs; v_neg = new_v_neg;
    }

    /* v_abs now contains |Im((-1-3i)^n)| */
    seq_t temp = v_abs;

    /* exponent of 2 in v_abs */
    idx_t v2 = 0;
    while (temp != (seq_t)0 && (temp & (seq_t)1) == (seq_t)0) {
        temp = temp / (seq_t)2;
        ++v2;
    }

    /* exponent of 5 in v_abs */
    temp = v_abs;
    idx_t v5 = 0;
    while (temp != (seq_t)0 && (temp % (seq_t)5) == (seq_t)0) {
        temp = temp / (seq_t)5;
        ++v5;
    }

    /* compute denominator a(n) = 5^n * 2^(n - min(v2, n)) */
    idx_t exp2 = (v2 < n) ? (n - v2) : (idx_t)0;
    idx_t exp5 = (v5 < n) ? (n - v5) : (idx_t)0;

    seq_t pow2 = seq_pow((seq_t)2, exp2);
    seq_t pow5 = seq_pow((seq_t)5, exp5);

    return pow2 * pow5;
}
