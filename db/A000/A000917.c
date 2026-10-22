/*
 * A000917  a(n) = (2n+3)!/(n!*(n+2)!).
 * https://oeis.org/A000917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n+3)!/(n!*(n+2)!)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t n_seq = (seq_t)n;
    for (idx_t i = 0; i <= n; i++) {
        result = result * (2 * n_seq + 3 - (seq_t)i) / ((seq_t)(i + 1));
    }
    return result * (n_seq + 1);
}
