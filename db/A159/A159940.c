/*
 * A159940  The number of trisubstitution products with composition C_n
 *          H_(2n-1) X_2 Y.
 * https://oeis.org/A159940
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - 9*n^2 + 19*n - 14 (for n ≥ 2)
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 1291, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        /* Offset of the sequence is 2; values below are undefined. */
        return (seq_t)0;
    }
    seq_t N = (seq_t)n;
    /* Compute the cubic polynomial using seq_t arithmetic only. */
    return 2 * N * N * N - 9 * N * N + 19 * N - 14;
}
