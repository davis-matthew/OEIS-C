/*
 * A175777  Partial sums of floor(n^2/16).
 * https://oeis.org/A175777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*(2*n^2 + 5*n - 15)/96) (valid for n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 4977, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special‑case the offset value. */
    if (n == 0) return 0;

    /* Promote the index to the sequence type to avoid signedness issues. */
    seq_t N   = (seq_t)n;          /* N = n  */
    seq_t Nm1 = N - 1;             /* N-1   */

    /* Compute the numerator of the fraction. */
    seq_t part = 2 * N * N + 5 * N - 15;   /* 2*n^2 + 5*n - 15 */
    seq_t num  = Nm1 * part;               /* (n-1)*(2*n^2 + 5*n - 15) */

    /* Integer division truncates toward zero; for a positive numerator
       this is the same as the floor function, giving the required result. */
    return num / 96;
}
