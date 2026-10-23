/*
 * A116165  a(n) = 7^n * n*(n+1).
 * https://oeis.org/A116165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n * n*(n+1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pw = (seq_t)1;                /* pw = 7^n */
    for (idx_t i = 0; i < n; ++i)
        pw *= (seq_t)7;                /* iterative power calculation */

    seq_t nn = (seq_t)n;                /* convert index to seq_t */
    return pw * nn * (nn + (seq_t)1);   /* 7^n * n * (n+1) */
}
