/*
 * A017538  a(n) = (12*n + 1)^6.
 * https://oeis.org/A017538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 1)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 135, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)12 + (seq_t)1;   /* 12*n + 1 */
    seq_t b2   = base * base;   /* (12*n + 1)^2 */
    seq_t b4   = b2   * b2;     /* (12*n + 1)^4 */
    seq_t b6   = b4   * b2;     /* (12*n + 1)^6 (final result) */
    return b6;
}
