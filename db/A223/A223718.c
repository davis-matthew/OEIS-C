/*
 * A223718  Number of unimodal functions [1..n]->[0..2].
 * https://oeis.org/A223718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*(n+1)*(n^2 +5*n+18)/24
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 564, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x    = (seq_t)n;          /* n */
    seq_t x1   = x + 1;             /* n+1 */
    seq_t x2   = x * x;             /* n^2 */
    seq_t term = x2 + 5 * x + (seq_t)18;   /* n^2 + 5*n + 18 */
    seq_t prod = x * x1;            /* n*(n+1) */
    prod      = prod * term;        /* * (n^2 + 5*n + 18) */
    seq_t res  = prod / (seq_t)24;  /* divide by 24 */
    res       = res + 1;            /* add the leading 1 */
    return res;
}
