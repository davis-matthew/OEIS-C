/*
 * A016988  a(n) = (7*n)^8.
 * https://oeis.org/A016988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 36, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute p = 7*n */
    seq_t p = (seq_t)7 * n;
    /* p^2, p^4, p^8 */
    seq_t p2 = p * p;
    seq_t p4 = p2 * p2;
    seq_t p8 = p4 * p4;
    return p8;
}
