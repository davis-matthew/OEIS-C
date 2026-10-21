/*
 * A227704  The hyper-Wiener index of the zig-zag polyhex nanotube
 *          TUHC_6[2n,2] defined pictorially in Fig. 1 of the Eliasi et al.
 *          reference.
 * https://oeis.org/A227704
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(15 + 14*n + 12*n^2 + 4*n^3)/3
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 190, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t term = (seq_t)15 + (seq_t)14 * nn;
    term += (seq_t)12 * n2;
    term += (seq_t)4 * n3;
    return nn * term / 3;
}
