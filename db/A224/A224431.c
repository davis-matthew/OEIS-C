/*
 * A224431  The Wiener index of the dendrimer D_2[n], defined pictorially in
 *          the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -14+4846*2^n+1056*2^n*n+2880*4^n*n-4608*4^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n) {
    /* Compute powers of 2 and 4 */
    seq_t pow2_n = pow2(n);
    seq_t pow4_n = pow4(n);

    /* Terms of the formula */
    seq_t term_A = (seq_t)4846 * pow2_n;
    seq_t term_B = (seq_t)1056 * (seq_t)n * pow2_n;
    seq_t term_C = (seq_t)2880 * (seq_t)n * pow4_n;
    seq_t term_D = (seq_t)4608 * pow4_n;

    /* Combine them, remembering the final subtraction */
    seq_t result = term_A + term_B + term_C - term_D - (seq_t)14;

    return result;
}
