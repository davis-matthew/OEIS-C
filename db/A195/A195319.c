/*
 * A195319  Three times second hexagonal numbers: 3*n*(2*n+1).
 * https://oeis.org/A195319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(2*n+1)
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_t = (seq_t)n;
    return n_t * (n_t * 2 + (seq_t)1) * (seq_t)3;
}
