/*
 * A204215  Number of length 7 nonnegative integer arrays starting and ending
 *          with 0 with adjacent elements differing by no more than n.
 * https://oeis.org/A204215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (176*n^5 + 665*n^4 + 1020*n^3 + 805*n^2 + 334*n + 60) /
 *            60
 * Range:     u8 none, u16 n <= 2, u32 n <= 59, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from empirical polynomial: a(n) = (44/15)*n^5 + (133/12)*n^4 + 17*n^3 + (161/12)*n^2 + (167/30)*n + 1 */

seq_t a(idx_t n) {
    /* OEIS offset is 1; for n==0 return 0 (undefined in OEIS) */
    if (n == 0) return (seq_t)0;

    seq_t N = (seq_t)n;           /* cast to seq_t before arithmetic */
    seq_t n2 = N * N;             /* n^2 */
    seq_t n3 = n2 * N;            /* n^3 */
    seq_t n4 = n3 * N;            /* n^4 */
    seq_t n5 = n4 * N;            /* n^5 */

    /* Integer numerator of the polynomial */
    seq_t num = (seq_t)176 * n5
              + (seq_t)665 * n4
              + (seq_t)1020 * n3
              + (seq_t)805 * n2
              + (seq_t)334 * N
              + (seq_t)60;

    return num / (seq_t)60;
}
