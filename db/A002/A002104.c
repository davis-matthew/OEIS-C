/*
 * A002104  Logarithmic numbers.
 * https://oeis.org/A002104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{m=0..n-1} A000522(m), where A000522(m) =
 *            m*A000522(m-1) + 1, A000522(0) = 1; a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t e = 1;
    for (idx_t m = 0; m < n; m++) {
        if (m > 0) e = e * (seq_t)m + 1;
        sum += e;
    }
    return sum;
}
