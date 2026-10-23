/*
 * A160117  Number of "ON" cells after n-th stage in simple 2-dimensional
 *          cellular automaton (see Comments for precise definition).
 * https://oeis.org/A160117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(1) = 1; a(2*n) = 5 + 2*n*(7*n-5) for n >= 1 a(2*n+1)
 *            = 5 + 2*n*(7*n-3) for n >= 1
 * Range:     u8 n <= 9, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if ((n & 1ULL) == 0) {
        idx_t k = n / 2;
        seq_t sk = (seq_t)k;
        return (seq_t)5 + (seq_t)2 * sk * ((seq_t)7 * sk - (seq_t)5);
    } else {
        idx_t k = (n - 1) / 2;
        seq_t sk = (seq_t)k;
        return (seq_t)5 + (seq_t)2 * sk * ((seq_t)7 * sk - (seq_t)3);
    }
}
