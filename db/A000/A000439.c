/*
 * A000439  Powers of rooted tree enumerator.
 * https://oeis.org/A000439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 18*n^3 + 83*n^2 + 114*n)/24
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    
    seq_t numerator = x4 + 18 * x3 + 83 * x2 + 114 * x;
    return numerator / 24;
}
