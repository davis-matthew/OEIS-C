/*
 * A087420  a(n) is the sum of the squares of the sizes of the conjugacy
 *          classes in the dihedral group D_2n.
 * https://oeis.org/A087420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2/2 + 2n - 2 if n is even * a(n) = n^2 + 2n - 1 if n is
 *            odd * This implements the explicit formula for OEIS A087420
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;               /* convert idx_t to seq_t for calculations */
    if ((n & 1) == 0) {                  /* even n */
        return (n_seq * n_seq) / 2 + (seq_t)2 * n_seq - 2;
    } else {                              /* odd n */
        return n_seq * n_seq + (seq_t)2 * n_seq - 1;
    }
}
