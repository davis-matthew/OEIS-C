/*
 * A000540  Sum of 6th powers: 0^6 + 1^6 + 2^6 + ... + n^6.
 * https://oeis.org/A000540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} i^6, computed term by term so no
 *            intermediate exceeds a(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 30, u64 n <= 745, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t c = si * si * si;
        sum += c * c;
    }
    return sum;
}
