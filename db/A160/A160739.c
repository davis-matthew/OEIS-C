/*
 * A160739  16*P_6(n), 16 times the Legendre Polynomial of order 6 at n.
 * https://oeis.org/A160739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 231*n^6 - 315*n^4 + 105*n^2 - 5
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 14, i64 n <= 584, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;
    seq_t n2 = sn * sn;
    seq_t n4 = n2 * n2;
    seq_t n6 = n4 * n2;
    return (seq_t)231 * n6 - (seq_t)315 * n4 + (seq_t)105 * n2 - (seq_t)5;
}
