/*
 * A004281  2, 4 and the odd numbers.
 * https://oeis.org/A004281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n+1 for n≤3, a(n) = 2n-3 for n>3
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 3) {
        return (seq_t)n + 1;
    } else {
        return 2 * (seq_t)n - 3;
    }
}
