/*
 * A006578  Triangular numbers plus quarter squares: n*(n+1)/2 + floor(n^2/4)
 *          (i.e., A000217(n) + A002620(n)).
 * https://oeis.org/A006578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 + floor(n^2/4)
 * Range:     u8 n <= 18, u16 n <= 295, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t part1 = (n_seq * (n_seq + 1)) / 2;
    seq_t part2 = (n_seq * n_seq) / 4;
    return part1 + part2;
}
