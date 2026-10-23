/*
 * A115519  a(n) = n*(1+3*n+6*n^2)/2.
 * https://oeis.org/A115519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(1+3*n+6*n^2)/2
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = sn * sn;           /* n^2 */
    seq_t expr = 1 + 3*sn + 6*n2; /* 1 + 3*n + 6*n^2 */
    seq_t res = sn * expr;        /* n * expr */
    res = res / 2;                /* divide by 2 (exact) */
    return res;
}
