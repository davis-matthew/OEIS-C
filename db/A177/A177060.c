/*
 * A177060  a(n) = (7*n+2)*(7*n+5) = 49*n^2 + 49*n + 10.
 * https://oeis.org/A177060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n+2)*(7*n+5) = 49*n^2 + 49*n + 10
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;          /* promote index to seq_t */
    seq_t term = 49 * n_val * n_val + 49 * n_val + 10;
    return term;
}
