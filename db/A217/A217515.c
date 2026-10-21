/*
 * A217515  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123)*.
 * https://oeis.org/A217515
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [6, 4, 3][(n + 1) mod 3]
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[3] = {(seq_t)6, (seq_t)4, (seq_t)3};
    return table[(n + 1) % 3];
}
