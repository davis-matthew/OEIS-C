/*
 * A004276  0, 2, 4 and the odd numbers.
 * https://oeis.org/A004276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n (n <= 5); 2*n - 5 (n > 5)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 5) {
        return (seq_t)n;
    } else {
        seq_t sn = (seq_t)n;
        return 2 * sn - 5;
    }
}
