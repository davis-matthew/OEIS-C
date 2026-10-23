/*
 * A153793  13 times pentagonal numbers: a(n) = 13*n*(3*n-1)/2.
 * https://oeis.org/A153793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n*(3*n-1)/2 (equivalently (39*n^2-13*n)/2)
 * Range:     u8 n <= 3, u16 n <= 58, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return ((seq_t)39 * sn * sn - (seq_t)13 * sn) / (seq_t)2;
}
