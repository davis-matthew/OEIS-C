/*
 * A185270  a(n) = 648 * n^6.
 * https://oeis.org/A185270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 648*n^6
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 13, u64 n <= 552, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 648;
    for (int i = 0; i < 6; ++i) {
        res *= nn;
    }
    return res;
}
