/*
 * A033591  a(n) = (2*n+1)*(3*n+1)*(4*n+1).
 * https://oeis.org/A033591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(3*n+1)*(4*n+1)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 563, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS offset: 0 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t part1 = tn * 2 + 1;
    seq_t part2 = tn * 3 + 1;
    seq_t part3 = tn * 4 + 1;
    return part1 * part2 * part3;
}
