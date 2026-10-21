/*
 * A228546  x-values in the solution to the Pell equation x^2 - 74*y^2 = -1.
 * https://oeis.org/A228546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7398*a(n-1)-a(n-2) (n≥3) Initial values: a(1) = 43, a(2)
 *            = 318157
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* value below offset is irrelevant */
    if (n == 1) return (seq_t)43;
    if (n == 2) return (seq_t)318157;

    /* iterative recurrence */
    seq_t a_prev2 = (seq_t)43;            /* a(1) */
    seq_t a_prev  = (seq_t)318157;        /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)7398 * a_prev - a_prev2;
        a_prev2 = a_prev;
        a_prev  = next;
    }
    return a_prev;
}
