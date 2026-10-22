/*
 * A003095  a(n) = a(n-1)^2 + 1 for n >= 1, with a(0) = 0.
 * https://oeis.org/A003095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = a(n-1)^2 + 1, a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t result = 0;
    for (idx_t i = 1; i <= n; i++) {
        result = result * result + 1;
    }
    return result;
}
