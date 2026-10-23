/*
 * A033570  Pentagonal numbers with odd index: a(n) = (2*n+1)*(3*n+1).
 * https://oeis.org/A033570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(3*n+1)
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t part1 = ns * 2 + 1;
    seq_t part2 = ns * 3 + 1;
    return part1 * part2;
}
