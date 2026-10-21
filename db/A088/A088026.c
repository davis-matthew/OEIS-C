/*
 * A088026  Number of "sets of even lists" for even n, cf. A000262.
 * https://oeis.org/A088026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2n-1)^2*a(n-1) - 4*(n-1)*(n-2)*(2n-1)*(2n-3)*a(n-2),
 *            with a(0)=1 and a(1)=2. This recurrence was given in the OEIS
 *            entry. The function recomputes a(n) from scratch; it uses only
 *            arithmetic on seq_t and idx_t and requires no external utilities
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* We keep only the last two computed values to save memory. */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)2;   /* a(1) */
    seq_t a_n   = 0;          /* placeholder for the current value */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute the polynomial coefficients as seq_t. */
        seq_t two_i_minus1 = (seq_t)(2 * i - 1);
        seq_t two_i_minus3 = (seq_t)(2 * i - 3);
        seq_t coeff1 = (seq_t)2 * two_i_minus1 * two_i_minus1;   /* 2*(2i-1)^2   */
        seq_t coeff2 = (seq_t)4 * (seq_t)(i - 1) * (seq_t)(i - 2)
                       * two_i_minus1 * two_i_minus3;           /* 4*(i-1)*(i-2)*(2i-1)*(2i-3) */

        a_n = coeff1 * a_nm1 - coeff2 * a_nm2;

        /* Shift for next iteration. */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
