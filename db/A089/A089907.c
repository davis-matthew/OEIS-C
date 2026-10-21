/*
 * A089907  Sum of digits of numbers between 0 and (6/9)*(10^n-1).
 * https://oeis.org/A089907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (-11/9*(-1+10^n)+(2+3*10^n)*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* Compute 10^n. */
    seq_t pow10_n = pow10(n);

    /* (10^n - 1) / 9, which is an integer. */
    seq_t div9 = (pow10_n - 1) / 9;

    /* term1 = 11 * ((10^n - 1) / 9) */
    seq_t term1 = 11 * div9;

    /* Convert n to seq_t for arithmetic. */
    seq_t n_seq = (seq_t)n;

    /* term2 = 3 * n * 10^n + 2 * n */
    seq_t term2 = (3 * n_seq) * pow10_n + 2 * n_seq;

    /* result = term2 - term1 */
    return term2 - term1;
}
