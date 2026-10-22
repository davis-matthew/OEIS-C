/*
 * A003009  a(n+1) = a(n) + 4^(n-1) a(n)^2.
 * https://oeis.org/A003009
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n) + 4^(n-1)*a(n)^2, a(3) = 3
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)3;
    seq_t p4 = (seq_t)16; /* 4^(i-1) for i = 3 */
    idx_t i;
    for (i = 3; i < n; i++) {
        result = result + p4 * result * result;
        p4 = p4 * 4;
    }
    return result;
}
