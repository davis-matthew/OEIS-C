/*
 * A168123  a(n) = n^2*(n^5+1)/2.
 * https://oeis.org/A168123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^5+1)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 624, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t n2 = nn * nn;                /* n^2  */
    seq_t n4 = n2 * n2;                /* n^4  */
    seq_t n5 = n4 * nn;                /* n^5  */

    if ((nn & (seq_t)1) == 0) {        /* n even  → divide n^2 by 2 */
        seq_t half_nn = nn >> 1;       /* n/2   */
        seq_t n2_half = half_nn * nn; /* n^2/2 */
        return n2_half * (n5 + (seq_t)1);
    } else {                           /* n odd   → divide (n^5+1) by 2 */
        seq_t half_term = (n5 + (seq_t)1) >> 1; /* (n^5+1)/2 */
        return half_term * n2;
    }
}
