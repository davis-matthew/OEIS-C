/*
 * A017488  a(n) = (11*n + 8)^4.
 * https://oeis.org/A017488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 22, u64 n <= 5957, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned __int128 x = (unsigned __int128)n * 11 + 8;
    x = x * x;  /* (11*n + 8)^2 */
    x = x * x;  /* (11*n + 8)^4 */
    return (seq_t)x;
}
