/*
 * A229745  a(n) = wt(n+wt(n))-wt(n), where wt(n) is the binary weight of n,
 *          A000120(n).
 * https://oeis.org/A229745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = popcount(n + popcount(n)) - popcount(n)
 * Range:     i8 n <= 536, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t popcount_seq_t(seq_t x)
{
    seq_t cnt = 0;
    /* loop while bits remain */
    while (x != 0)
    {
        x &= (x - 1);   /* clears the lowest set bit */
        cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;                   /* cast index to seq_t */
    seq_t wt_n = popcount_seq_t(nt);       /* weight of n */
    seq_t w = nt + wt_n;                   /* n + wt(n) */
    seq_t wt_w = popcount_seq_t(w);        /* weight of n + wt(n) */
    return wt_w - wt_n;                    /* final value */
}
