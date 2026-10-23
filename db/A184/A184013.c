/*
 * A184013  a(n) = n - 1 + ceiling((4+n^2)/5); complement of A184012.
 * https://oeis.org/A184013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + ceil((n^2 + 4)/5) = n - 1 + (n^2 + 8)/5
 * Range:     u8 n <= 33, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = sn - 1 + (sn * sn + (seq_t)8) / (seq_t)5;
    return res;
}
