/*
 * A003010  A Lucas-Lehmer sequence: a(0) = 4; for n>0, a(n) = a(n-1)^2 - 2.
 * https://oeis.org/A003010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)^2 - 2, a(0) = 4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)4;
    
    seq_t result = (seq_t)4;
    for (idx_t i = 1; i <= n; i++) {
        result = result * result - (seq_t)2;
    }
    return result;
}
