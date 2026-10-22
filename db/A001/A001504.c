/*
 * A001504  a(n) = (3*n+1)*(3*n+2).
 * https://oeis.org/A001504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)*(3*n+2)
 * Range:     u8 n <= 4, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    return (3 * n_s + 1) * (3 * n_s + 2);
}
