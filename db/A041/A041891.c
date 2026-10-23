/*
 * A041891  Denominators of continued fraction convergents to sqrt(467).
 * https://oeis.org/A041891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(467). */
#include <stdint.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Continued fraction coefficients for sqrt(467):
       a0 = 21
       period (a1 … a14) = [1,1,1,1,3,3,21,3,3,1,1,1,1,42] */
    static const seq_t period[14] = {
        1, 1, 1, 1, 3, 3, 21,
        3, 3, 1, 1, 1, 1, 42
    };

    /* q(-2) = 1, q(-1) = 0  (denominators of successive convergents) */
    seq_t q_nm2 = (seq_t)1;
    seq_t q_nm1 = (seq_t)0;
    seq_t q_n   = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)21;          /* a0 */
        } else {
            a_k = period[(k - 1) % 14]; /* periodic part */
        }
        q_n = a_k * q_nm1 + q_nm2;
        q_nm2 = q_nm1;
        q_nm1 = q_n;
    }
    return q_n; /* denominator of the n‑th convergent */
}
