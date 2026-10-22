/*
 * A001715  a(n) = n!/6.
 * https://oeis.org/A001715
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!/6 = 4*5*...*n, with a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t i;
    for (i = 4; i <= n; i++) {
        result *= (seq_t)i;
    }
    return result;
}
