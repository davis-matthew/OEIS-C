/*
 * A004272  1, 3, 5 and the nonnegative even numbers.
 * https://oeis.org/A004272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n for n < 7; a(n) = 2*n - 6 for n >= 7
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if (n < 7) {
        return sn;
    } else {
        return 2 * sn - 6;
    }
}
