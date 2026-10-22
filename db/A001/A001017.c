/*
 * A001017  Ninth powers: a(n) = n^9.
 * https://oeis.org/A001017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t s2 = sn * sn;
    seq_t s4 = s2 * s2;
    seq_t s8 = s4 * s4;
    return s8 * sn;
}
