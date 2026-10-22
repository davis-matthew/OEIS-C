/*
 * A004927  a(n) = floor(n*phi^12), where phi is the golden ratio, A001622.
 * https://oeis.org/A004927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * phi^12), where phi^12 = 161 + 72*sqrt(5)
 * Range:     u8 n <= 0, u16 n <= 203, u32 n <= 1721, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t part1 = (seq_t)161 * n_seq;
    seq_t part2 = ((seq_t)72 * (seq_t)51841 * n_seq) / (seq_t)23184;
    return part1 + part2;
}
