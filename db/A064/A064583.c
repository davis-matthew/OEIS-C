/*
 * A064583  a(n) = n^4*(n^4+1)*(n^2-1).
 * https://oeis.org/A064583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4*(n^4+1)*(n^2-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* a(0) and a(1) are zero */
    seq_t s  = (seq_t)n;                 /* cast index to seq_t */
    seq_t n2 = s * s;                    /* n^2 */
    seq_t n4 = n2 * n2;                  /* n^4 */
    return n4 * (n4 + (seq_t)1) * (n2 - (seq_t)1);
}
