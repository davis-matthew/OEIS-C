/*
 * A158587  a(n) = 289*n^2 - 17.
 * https://oeis.org/A158587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 289*n^2 - 17
 * Range:     u8 none, u16 n <= 15, u32 n <= 3855, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return 289 * nn * nn - 17;
}
