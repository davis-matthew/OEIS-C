/*
 * A003600  Maximal number of pieces obtained by slicing a torus (or a bagel)
 *          with n cuts: (n^3 + 3*n^2 + 8*n)/6 (n > 0).
 * https://oeis.org/A003600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n^3 + 3*n^2 + 8*n)/6 for n > 0; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    
    return (n3 + 3 * n2 + 8 * n_seq) / 6;
}
