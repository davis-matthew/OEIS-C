/*
 * A047246  Numbers that are congruent to {0, 1, 2, 3} mod 6.
 * https://oeis.org/A047246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((6/5)*floor(5*(n-1)/4))
 * Range:     u8 n <= 172, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t k = (seq_t)5 * (sn - 1) / 4;
    seq_t res = (seq_t)6 * k / 5;
    return res;
}
