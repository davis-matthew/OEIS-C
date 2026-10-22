/*
 * A000340  a(0)=1, a(n) = 3*a(n-1) + n + 1.
 * https://oeis.org/A000340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + n + 1, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        v = v * 3 + (seq_t)i + 1;
    }
    return v;
}
