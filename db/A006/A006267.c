/*
 * A006267  Continued cotangent for the golden ratio.
 * https://oeis.org/A006267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)^3 + 3*a(n-1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    for (idx_t i = 0; i < n; i++) {
        v = v * v * v + 3 * v;
    }
    return v;
}
