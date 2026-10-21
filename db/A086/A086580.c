/*
 * A086580  a(n) = 9*(10^n - 1).
 * https://oeis.org/A086580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*(10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;
    for (idx_t i = 0; i < n; ++i) {
        r *= 10;
    }
    r -= 1;
    r *= 9;
    return r;
}
