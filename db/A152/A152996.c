/*
 * A152996  9 times pentagonal numbers: 9*n*(3*n-1)/2.
 * https://oeis.org/A152996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*n*(3*n-1)/2
 * Range:     u8 n <= 3, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t val = nn * ((seq_t)3 * nn - 1);
    val = val * 9;
    val = val / 2;
    return val;
}
