/*
 * A219164  Recurrence equation a(n+1) = a(n)^4 - 4*a(n)^2 + 2 with a(0) = 5.
 * https://oeis.org/A219164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n)^4 - 4*a(n)^2 + 2, with a(0) = 5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)5;
    for (idx_t i = 0; i < n; ++i) {
        seq_t x2 = x * x;
        seq_t x4 = x2 * x2;
        x = x4 - (seq_t)4 * x2 + (seq_t)2;
    }
    return x;
}
