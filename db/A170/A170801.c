/*
 * A170801  a(n) = n^10*(n^9 + 1)/2.
 * https://oeis.org/A170801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10*(n^9 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    /* compute n^9 */
    seq_t sn9 = sn;
    for (idx_t i = 1; i < 9; ++i) {
        sn9 *= sn;
    }
    /* compute n^10 = n^9 * n */
    seq_t sn10 = sn9 * sn;
    /* compute (n^10)*(n^9 + 1)/2 */
    seq_t result = (sn10 * (sn9 + 1)) / 2;
    return result;
}
