/*
 * A194476  Number of ways to arrange 4 indistinguishable points on an n X n X
 *          n triangular grid so that no three points are in the same row or
 *          diagonal.
 * https://oeis.org/A194476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/384)*n^8 + (1/96)*n^7 - (5/64)*n^6 + (13/240)*n^5 * +
 *            (27/128)*n^4 - (23/96)*n^3 - (13/96)*n^2 + (7/40)*n *
 *            Equivalently a(n) = [5 n^8 + 20 n^7 - 150 n^6 + 104 n^5 + 405
 *            n^4 * - 460 n^3 - 260 n^2 + 336 n] / 1920
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 112, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;
    seq_t n7 = n6 * nn;
    seq_t n8 = n7 * nn;

    seq_t pos = 5 * n8 + 20 * n7 + 104 * n5 + 405 * n4 + 336 * nn;
    seq_t neg = 150 * n6 + 460 * n3 + 260 * n2;

    seq_t q = pos - neg;
    return q / 1920;
}
