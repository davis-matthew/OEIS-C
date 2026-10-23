/*
 * A168192  a(n) = n^4*(n^2 + 1)/2.
 * https://oeis.org/A168192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(n^2 + 1)/2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2     = n_seq * n_seq;       /* n^2          */
    seq_t n4     = n2 * n2;             /* n^4          */
    seq_t term1, term2;                 /* one divided by 2 */

    /* One of n4 or (n2+1) is even: divide the even one to avoid overflow */
    if ((n4 & 1) == 0) {
        term1 = n4 >> 1;        /* n4 / 2          */
        term2 = n2 + 1;         /* n^2 + 1         */
    } else {
        term1 = n4;             /* n4              */
        term2 = (n2 + 1) >> 1;  /* (n^2 + 1) / 2   */
    }

    return term1 * term2;
}
