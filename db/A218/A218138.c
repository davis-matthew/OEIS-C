/*
 * A218138  Sum of absolute values of real and imaginary parts of the
 *          coefficients in the expansion of 1 / (1 - x - 2*I*x^2), where
 *          I^2=-1.
 * https://oeis.org/A218138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; for n≥2 compute coefficients c_n of *
 *            1/(1‑x‑2*i*x^2) via c_n = c_{n‑1} + 2i*c_{n‑2} (c_0=c_1=1) * and
 *            return |Re(c_n)| + |Im(c_n)|
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 73, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;

    /*  r_mag / r_sign  represents real part of c_{k}
     *  s_mag / s_sign  represents imaginary part of c_{k}   */
    seq_t r_prev1_mag = (seq_t)1, s_prev1_mag = (seq_t)0;
    seq_t r_prev2_mag = (seq_t)1, s_prev2_mag = (seq_t)0;
    bool r_prev1_sign = false, s_prev1_sign = false;
    bool r_prev2_sign = false, s_prev2_sign = false;

    for (idx_t k = 2; k <= n; ++k) {
        /* ---------- compute new real part ---------- */
        seq_t b_mag_r = s_prev2_mag + s_prev2_mag;  /* 2 * |s_{k-2}| */
        bool   b_sign_r = !s_prev2_sign;            /* negative of s_{k-2} */

        seq_t r_new_mag;
        bool   r_new_sign;
        if (r_prev1_sign == b_sign_r) {          /* same sign: add magnitudes */
            r_new_sign = r_prev1_sign;
            r_new_mag = r_prev1_mag + b_mag_r;
        } else {                                 /* different signs: subtract */
            if (r_prev1_mag >= b_mag_r) {
                r_new_sign = r_prev1_sign;
                r_new_mag = r_prev1_mag - b_mag_r;
            } else {
                r_new_sign = b_sign_r;
                r_new_mag = b_mag_r - r_prev1_mag;
            }
        }

        /* ---------- compute new imaginary part ---------- */
        seq_t b_mag_s = r_prev2_mag + r_prev2_mag; /* 2 * |r_{k-2}| */
        bool   b_sign_s = r_prev2_sign;           /* same sign as r_{k-2} */

        seq_t s_new_mag;
        bool   s_new_sign;
        if (s_prev1_sign == b_sign_s) {           /* same sign: add magnitudes */
            s_new_sign = s_prev1_sign;
            s_new_mag = s_prev1_mag + b_mag_s;
        } else {                                 /* different signs: subtract */
            if (s_prev1_mag >= b_mag_s) {
                s_new_sign = s_prev1_sign;
                s_new_mag = s_prev1_mag - b_mag_s;
            } else {
                s_new_sign = b_sign_s;
                s_new_mag = b_mag_s - s_prev1_mag;
            }
        }

        /* shift previous values for next iteration */
        r_prev2_mag = r_prev1_mag; r_prev2_sign = r_prev1_sign;
        s_prev2_mag = s_prev1_mag; s_prev2_sign = s_prev1_sign;
        r_prev1_mag = r_new_mag; r_prev1_sign = r_new_sign;
        s_prev1_mag = s_new_mag; s_prev1_sign = s_new_sign;
    }

    /* a(n) is the sum of the magnitudes of the final real and imaginary parts */
    return r_prev1_mag + s_prev1_mag;
}
