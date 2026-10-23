/*
 * A156640  a(n) = 169*n^2 + 140*n + 29.
 * https://oeis.org/A156640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 169*n^2 + 140*n + 29
 * Range:     u8 n <= 0, u16 n <= 19, u32 n <= 5040, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = (seq_t)169 * nn * nn + (seq_t)140 * nn + 29;
    return res;
}
