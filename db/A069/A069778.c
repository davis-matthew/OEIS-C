/*
 * A069778  q-factorial numbers 3!_q.
 * https://oeis.org/A069778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(n^2 + n + 1)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1624, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t part1 = x + 1;
    seq_t part2 = x * x + x + 1;
    return part1 * part2;
}
