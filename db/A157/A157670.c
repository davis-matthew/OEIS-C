/*
 * A157670  a(n) = 531441*n^2 - 322218*n + 48842.
 * https://oeis.org/A157670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 531441*n^2 - 322218*n + 48842
 * Range:     u8 none, u16 none, u32 n <= 90, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t res = 531441 * n2 - 322218 * sn + 48842;
    return res;
}
