/*
 * A146534  a(n) = 4*C(2n,n) - 3*0^n.
 * https://oeis.org/A146534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*C(2n,n) - 3*0^n (so a(0)=1, a(n>0)=4*C(2n,n))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute binomial coefficient C(2n,n) multiplicatively */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = binom * (seq_t)(n + i);   /* multiply by (n+i) */
        binom = binom / (seq_t)i;          /* divide by i, integer division */
    }
    seq_t res = binom * 4;
    if (n == 0)
        res -= 3;        /* subtract 3*0^0 = 3 */
    return res;
}
