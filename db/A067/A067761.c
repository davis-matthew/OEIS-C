/*
 * A067761  Positive integers divisible by 5 but not by 7.
 * https://oeis.org/A067761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n + 5*⌊(n-1)/6⌋, for n ≥ 1. This gives the positive
 *            integers divisible by 5 but not by 7
 * Range:     u8 n <= 44, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for offset 1, but safe */
    seq_t mult5   = (seq_t)5 * (seq_t)n;
    seq_t extra   = (seq_t)5 * ((n - 1) / 6);
    return mult5 + extra;
}
