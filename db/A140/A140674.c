/*
 * A140674  a(n) = n*(3*n + 17)/2.
 * https://oeis.org/A140674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n + 17)/2
 * Range:     u8 n <= 10, u16 n <= 206, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;
    return nn * (nn * 3 + 17) / 2;
}
