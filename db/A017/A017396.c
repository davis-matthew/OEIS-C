/*
 * A017396  a(n) = (11*n)^8.
 * https://oeis.org/A017396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 23, u128 n <= 5957,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * (seq_t)n;
    seq_t p2 = base * base;   /* (11n)^2 */
    seq_t p4 = p2 * p2;       /* (11n)^4 */
    seq_t p8 = p4 * p4;       /* (11n)^8 */
    return p8;
}
