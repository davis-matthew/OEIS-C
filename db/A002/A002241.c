/*
 * A002241  10th powers written backwards.
 * https://oeis.org/A002241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse(n^10)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 81, u128 n <= 6311,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute n^10
    seq_t base = (seq_t)n;
    seq_t power = base;
    for (idx_t i = 1; i < 10; i++) {
        power = power * base;
    }
    
    // Reverse the digits
    seq_t reversed = 0;
    while (power > 0) {
        reversed = reversed * 10 + (power % 10);
        power /= 10;
    }
    
    return reversed;
}
