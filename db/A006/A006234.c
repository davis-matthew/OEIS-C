/*
 * A006234  a(n) = n*3^(n-4).
 * https://oeis.org/A006234
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n*3^(n-4) for n >= 4, a(3) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 3) {
        return 1;
    }
    seq_t r = (seq_t)n;
    idx_t i;
    for (i = 4; i < n; i++) {
        r *= 3;
    }
    return r;
}
