/*
 * A007019  a(n) = (2n+1)! / 2^n.
 * https://oeis.org/A007019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (2n+1)! / 2^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t j = 1; j <= n; j++) {
        result = result * (seq_t)j * (2 * (seq_t)j + 1);
    }
    return result;
}
