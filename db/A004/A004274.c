/*
 * A004274  0, 2 and the odd numbers.
 * https://oeis.org/A004274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0; a(n) = 2*n - 3 + floor(2/n) for n > 0
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t sn = (seq_t)n;
    return 2 * sn - 3 + (2 / sn);
}
