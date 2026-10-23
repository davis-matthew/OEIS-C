/*
 * A155883  a(n) = 14*n^3 - 30*n^2 + 24*n - 7.
 * https://oeis.org/A155883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*n^3 - 30*n^2 + 24*n - 7
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 675, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 */

seq_t a(idx_t n) {
    /* Convert index to seq_t so all arithmetic is performed in seq_t. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t term = 14 * n3 - 30 * n2 + 24 * nn - 7;
    return term;
}
