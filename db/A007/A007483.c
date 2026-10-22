/*
 * A007483  a(n) = 3*a(n-1) + 2*a(n-2), with a(0)=1, a(1)=5.
 * https://oeis.org/A007483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2), a(0) = 1, a(1) = 5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 1, y = 5;
    if (n == 0) return x;
    for (idx_t i = 1; i < n; i++) {
        seq_t t = (seq_t)3 * y + (seq_t)2 * x;
        x = y;
        y = t;
    }
    return y;
}
