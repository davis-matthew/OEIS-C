/*
 * A002697  a(n) = n*4^(n-1).
 * https://oeis.org/A002697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*4^(n-1), a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t r = (seq_t)n;
    seq_t b = 4;
    idx_t e = n - 1;
    while (e > 0) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e > 0) b *= b;
    }
    return r;
}
