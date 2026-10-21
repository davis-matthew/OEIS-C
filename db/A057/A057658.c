/*
 * A057658  a(n) = n*(n+1)^2*(n+2)^3*(n+3)^2*(n+4).
 * https://oeis.org/A057658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(n+2)^3*(n+3)^2*(n+4)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 9, u64 n <= 136, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t   = (seq_t)n;          /* n */
    seq_t n1    = n_t + (seq_t)1;    /* n+1 */
    seq_t n2    = n_t + (seq_t)2;    /* n+2 */
    seq_t n3    = n_t + (seq_t)3;    /* n+3 */
    seq_t n4    = n_t + (seq_t)4;    /* n+4 */

    return n_t * n1 * n1 * n2 * n2 * n2 * n3 * n3 * n4;
}
