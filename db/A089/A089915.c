/*
 * A089915  Special values of generalized Laguerre polynomials L_n^(alpha)(x):
 *          a(n) = 4^n *n! *L_n^(-1/4)(-1).
 * https://oeis.org/A089915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 7 a(n) = (8*n - 1) * a(n-1) - 4*(n-1)*(4*n - 5)
 *            * a(n-2) (n ≥ 2) (This is the recurrence for OEIS A089915.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle initial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* Iterative computation using the recurrence */
    seq_t prev2 = (seq_t)1;        /* a(n-2) */
    seq_t prev1 = (seq_t)7;        /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff1 = (seq_t)8 * (seq_t)i - (seq_t)1;                     /* 8*n - 1   */
        seq_t coeff2 = ((seq_t)4 * (seq_t)(i - 1)) * (((seq_t)4 * (seq_t)i) - (seq_t)5); /* 4*(n-1)*(4*n-5) */
        curr = coeff1 * prev1 - coeff2 * prev2;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;   /* a(n) */
}
