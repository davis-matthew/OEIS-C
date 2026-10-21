/*
 * A088209  Numerators of convergents of the continued fraction with the n+1
 *          partial quotients: [1;1,1,...(n 1's)...,1,n+1], starting with [1],
 *          [1;2], [1;1,3], [1;1,1,4], ...
 * https://oeis.org/A088209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + Lucas(n), with a(0)=1, a(1)=3.
 *            Lucas(0)=2, Lucas(1)=1 and Lucas(k)=Lucas(k-1)+Lucas(k-2)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a_prev2 = (seq_t)1;      /* a(0) */
    seq_t a_prev1 = (seq_t)3;      /* a(1) */
    seq_t luc_prev2 = (seq_t)2;    /* Lucas(0) */
    seq_t luc_prev1 = (seq_t)1;    /* Lucas(1) */
    seq_t a_cur, luc_cur;

    idx_t i;
    for (i = 2; i <= n; ++i) {
        luc_cur = luc_prev1 + luc_prev2;    /* Lucas(i) */
        a_cur   = a_prev1 + a_prev2 + luc_cur;

        /* shift for next iteration */
        a_prev2  = a_prev1;
        a_prev1  = a_cur;
        luc_prev2 = luc_prev1;
        luc_prev1 = luc_cur;
    }

    return a_prev1;   /* a(n) */
}
