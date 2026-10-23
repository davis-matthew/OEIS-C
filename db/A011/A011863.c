/*
 * A011863  Nearest integer to (n/2)^4.
 * https://oeis.org/A011863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to (n/2)^4 (equivalently, a(n) =
 *            floor(n^4 / 16) )
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 302, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;   /* convert index to seq_t first */
    seq_t n2 = n0 * n0;    /* n^2 */
    seq_t n4 = n2 * n2;    /* n^4 */
    return n4 >> 4;        /* divide by 16, equivalent to floor(n^4/16) */
}
