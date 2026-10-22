/*
 * A005564  Number of n-step walks on square lattice in the first quadrant
 *          which finish at distance n-3 from the x-axis.
 * https://oeis.org/A005564
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = (n-2)*n*(n+1)/2
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    if (n_seq & 1) {
        return (n_seq - 2) * n_seq * ((n_seq + 1) / 2);
    } else {
        return (n_seq - 2) * (n_seq / 2) * (n_seq + 1);
    }
}
