/*
 * A173089  a(n) = 25*n^2 + n.
 * https://oeis.org/A173089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2 + n
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 1 using only integer arithmetic. */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;               /* Convert index to seq_t */
    return N * ( ((seq_t)25 * N) + (seq_t)1 );   /* N*(25*N+1) = 25*n^2 + n */
}
