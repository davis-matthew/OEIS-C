/*
 * A081599  Let n = 10x + y where 0 <= y <= 9, x >= 0. Then a(n) = 8x+y.
 * https://oeis.org/A081599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 2*floor(n/10)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is unsigned; floor division by 10 is just integer division. */
    return (seq_t)n - 2 * (seq_t)(n / 10);
}
