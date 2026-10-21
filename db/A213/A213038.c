/*
 * A213038  a(n) = n^2 - 3*floor(n/2)^2.
 * https://oeis.org/A213038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - 3*floor(n/2)^2
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* integer division by 2 is the same as shifting right by 1 */
    idx_t half = n >> 1;
    seq_t n_sq  = (seq_t)n * (seq_t)n;
    seq_t h_sq  = (seq_t)half * (seq_t)half;
    return n_sq - 3 * h_sq;
}
