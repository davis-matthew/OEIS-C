/*
 * A017264  a(n) = (9*n + 8)^8.
 * https://oeis.org/A017264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 27, u128 n <= 7280,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = ((seq_t)n * 9) + 8; /* (9*n + 8) */
    seq_t x2 = x * x;            /* (9*n + 8)^2 */
    seq_t x4 = x2 * x2;          /* (9*n + 8)^4 */
    return x4 * x4;              /* (9*n + 8)^8 */
}
