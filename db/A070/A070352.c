/*
 * A070352  a(n) = 3^n mod 5; or period 4, repeat [1, 3, 4, 2].
 * https://oeis.org/A070352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3^n mod 5
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t tbl[4] = {1, 3, 4, 2};
    return tbl[n % 4];
}
