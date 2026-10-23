/*
 * A131080  Periodic sequence (4, 4, 3, 1, 0, 0, 1, 3).
 * https://oeis.org/A131080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-8) for n>8; a(1..8) = 4, 4, 3, 1, 0, 0, 1, 3
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* periodic pattern of length 8 */
    static const seq_t pattern[8] = {4, 4, 3, 1, 0, 0, 1, 3};
    /* n is guaranteed to be >= 1; reduce by 1 to get 0‑based index */
    idx_t r = (n - 1) % 8;          /* r in [0,7] */
    return pattern[r];
}
