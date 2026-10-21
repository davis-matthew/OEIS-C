/*
 * A230623  Values of y such that x^2 + y^2 = 17^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A230623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(|Re((1+4i)^n)|, |Im((1+4i)^n)|) The values are
 *            obtained from the recurrence (a_{k+1},b_{k+1}) = (a_k - 4b_k,
 *            4a_k + b_k) with (a_1,b_1) = (1,4), keeping track of signs to
 *            avoid negative intermediate results
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1 – unused for n=0 */

    /* Initial components of (1+4i)^1 */
    seq_t a_abs = (seq_t)1;        /* |Re| */
    seq_t b_abs = (seq_t)4;        /* |Im| */
    int a_sign =  1;               /* +1 */
    int b_sign =  1;               /* +1 */

    for (idx_t i = 1; i < n; ++i) {
        /* Pre‑compute 4 * abs values */
        seq_t four_a = a_abs * (seq_t)4;
        seq_t four_b = b_abs * (seq_t)4;

        /* Next a component */
        seq_t a_next_abs;
        int a_next_sign;
        if (a_sign == -b_sign) {
            /* signs opposite → sum, sign follows a_sign */
            a_next_sign = a_sign;
            a_next_abs = a_abs + four_b;
        } else {
            /* signs equal → difference */
            if (a_abs >= four_b) {
                a_next_sign = a_sign;
                a_next_abs = a_abs - four_b;
            } else {
                a_next_sign = -a_sign;
                a_next_abs = four_b - a_abs;
            }
        }

        /* Next b component */
        seq_t b_next_abs;
        int b_next_sign;
        if (a_sign == b_sign) {
            /* signs equal → sum, sign follows a_sign */
            b_next_sign = a_sign;
            b_next_abs = four_a + b_abs;
        } else {
            /* signs opposite → difference */
            if (four_a >= b_abs) {
                b_next_sign = a_sign;
                b_next_abs = four_a - b_abs;
            } else {
                b_next_sign = b_sign;
                b_next_abs = b_abs - four_a;
            }
        }

        /* Update for next iteration */
        a_abs   = a_next_abs;
        a_sign  = a_next_sign;
        b_abs   = b_next_abs;
        b_sign  = b_next_sign;
    }

    /* Return the larger absolute component as the y‑value */
    return (a_abs > b_abs) ? a_abs : b_abs;
}
