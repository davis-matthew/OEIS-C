/*
 * A000407  a(n) = (2*n+1)! / n!.
 * https://oeis.org/A000407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2*n+1)! / n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t n_seq = (seq_t)n;
    for (seq_t k = n_seq + 1; k <= 2 * n_seq + 1; k++) {
        result *= k;
    }
    return result;
}
