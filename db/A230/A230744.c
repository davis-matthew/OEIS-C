/*
 * A230744  Values of y such that x^2 + y^2 = 41^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A230744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(|Re((4+5i)^n)|, |Im((4+5i)^n)|)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Below offset, not used */

    /* Start with (4,5) for n = 1 */
    seq_t abs_a = (seq_t)4;
    bool   sign_a = true;   /* true = +, false = - */
    seq_t abs_b = (seq_t)5;
    bool   sign_b = true;

    if (n == 1)
        return abs_b;  /* max(4,5) = 5 */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute new a = 4*a - 5*b */
        seq_t mag1_a = (seq_t)4 * abs_a;   /* 4 * |a| */
        seq_t mag2_a = (seq_t)5 * abs_b;   /* 5 * |b| */
        seq_t new_abs_a;
        bool   new_sign_a;

        if (sign_a == sign_b) {
            /* a and b have the same sign */
            if (mag1_a >= mag2_a) {
                new_abs_a = mag1_a - mag2_a;
                new_sign_a = sign_a;
            } else {
                new_abs_a = mag2_a - mag1_a;
                new_sign_a = !sign_a;
            }
        } else {
            /* a and b have opposite signs */
            new_abs_a = mag1_a + mag2_a;
            new_sign_a = sign_a;   /* sign of the larger term */
        }

        /* Compute new b = 5*a + 4*b */
        seq_t mag1_b = (seq_t)5 * abs_a;   /* 5 * |a| */
        seq_t mag2_b = (seq_t)4 * abs_b;   /* 4 * |b| */
        seq_t new_abs_b;
        bool   new_sign_b;

        if (sign_a == sign_b) {
            /* a and b have the same sign */
            new_abs_b = mag1_b + mag2_b;
            new_sign_b = sign_a;
        } else {
            /* a and b have opposite signs */
            if (mag1_b >= mag2_b) {
                new_abs_b = mag1_b - mag2_b;
                new_sign_b = sign_a;
            } else {
                new_abs_b = mag2_b - mag1_b;
                new_sign_b = !sign_a;
            }
        }

        /* Update state for next iteration */
        abs_a  = new_abs_a;
        sign_a = new_sign_a;
        abs_b  = new_abs_b;
        sign_b = new_sign_b;
    }

    return (abs_a > abs_b) ? abs_a : abs_b;
}
