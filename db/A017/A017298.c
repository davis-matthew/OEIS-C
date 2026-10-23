/*
 * A017298  a(n) = (10*n + 2)^6.
 * https://oeis.org/A017298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 2)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 162, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)10 * (seq_t)n + (seq_t)2;
    seq_t res   = 1;
    for (int i = 0; i < 6; ++i)
        res *= base;
    return res;
}
