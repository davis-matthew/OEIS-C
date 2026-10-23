/*
 * A160538  a(n) = 4*(n^4-n^3).
 * https://oeis.org/A160538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n^4-n^3)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;     /* convert index to seq_t */
    seq_t n2 = x * x;        /* n^2   */
    seq_t n3 = n2 * x;       /* n^3   */
    seq_t n4 = n3 * x;       /* n^4   */
    seq_t res = (seq_t)4 * (n4 - n3);   /* 4*(n^4-n^3) */
    return res;
}
