/*
 * A090757  Duplicate of A048739.
 * https://oeis.org/A090757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=1; for n≥1, a(n)=a(n-1)+P_{n+1} where Pell numbers P_k
 *            satisfy P_0=0, P_1=1, P_k=2·P_{k-1}+P_{k-2}
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;                  /* a(0)  */
    seq_t d_prevprev = (seq_t)2;             /* D_1 = a(1)-a(0) = P_2 */
    a_val += d_prevprev;                     /* a(1)   */

    if (n == 1)
        return a_val;

    seq_t d_prev = (seq_t)5;                 /* D_2 = a(2)-a(1) = P_3 */
    a_val += d_prev;                         /* a(2)   */

    for (idx_t i = 3; i <= n; ++i) {
        /* D_k = 2*D_{k-1} + D_{k-2}  */
        seq_t d = (seq_t)2 * d_prev + d_prevprev;
        a_val += d;
        d_prevprev = d_prev;
        d_prev = d;
    }

    return a_val;
}
