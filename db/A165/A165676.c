/*
 * A165676  Fourth right hand column of triangle A165674.
 * https://oeis.org/A165676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6 + 22*n + 18*n^2 + 4*n^3
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t res = 6;
    res += 22 * n_seq;
    res += 18 * n_seq * n_seq;
    res += 4 * n_seq * n_seq * n_seq;
    return res;
}
