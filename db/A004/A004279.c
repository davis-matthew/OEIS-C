/*
 * A004279  1, 3, 5 and the positive even numbers.
 * https://oeis.org/A004279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n+1 if n <= 5, else 2n-4
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 5) {
        return (seq_t)n + 1;
    } else {
        return 2 * (seq_t)n - 4;
    }
}
