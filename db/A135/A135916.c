/*
 * A135916  a(n) = (n^4 - 10*n^2 + 15*n - 6)/2.
 * https://oeis.org/A135916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 10*n^2 + 15*n - 6)/2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the OEIS sequence A135916.  */
/* a(n) = (n^4 - 10*n^2 + 15*n - 6)/2,  indexed from n = 1. */
seq_t a(idx_t n) {
    seq_t N = (seq_t)n;

    /* Compute n^4 */
    seq_t n2 = N * N;          /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */

    /* Compute -10*n^2 */
    seq_t term = n4;
    term -= ((seq_t)10 * n2);

    /* Compute +15*n */
    term += ((seq_t)15 * N);

    /* Compute -6 */
    term -= (seq_t)6;

    /* Divide by 2 */
    return term / (seq_t)2;
}
