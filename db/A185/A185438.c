/*
 * A185438  a(n) = 8*n^2 - 2*n + 1.
 * https://oeis.org/A185438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 2*n + 1
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;            /* convert index to seq_t */
    seq_t res = n_t * n_t;           /* n^2 */
    res = res * 8;                   /* 8*n^2 */
    res = res - n_t * 2;             /* subtract 2*n */
    return res + 1;                  /* add 1 */
}
