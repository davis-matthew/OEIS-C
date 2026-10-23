/*
 * A017112  a(n) = (8*n+3)^12.
 * https://oeis.org/A017112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 4, u128 n <= 202,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)8 * (seq_t)n + (seq_t)3;
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < 12; ++i)
        res *= t;
    return res;
}
