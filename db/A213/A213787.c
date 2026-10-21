/*
 * A213787  a(n) = Sum_{1<=i<j<k<=n} F(i)*F(j)*F(k), where F(m) is the m-th
 *          Fibonacci number.
 * https://oeis.org/A213787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1<=i<j<k<=n} F(i)*F(j)*F(k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t f_prev = 0;   /* F(0) */
    seq_t f_curr = 1;   /* F(1) */
    seq_t S = 0;        /* Sum of F(1)..F(i-1) */
    seq_t P2 = 0;       /* Sum of F(i)*F(j) for i<j<k (k = current i) */
    seq_t A = 0;        /* Result */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t f_i = f_curr;                 /* Current Fibonacci number */

        A += f_i * P2;   /* Add F(i) * P2(i-1) to answer */
        P2 += f_i * S;   /* Update pairwise sum for next step */
        S += f_i;        /* Update sum of Fibonacci numbers */

        /* Advance to next Fibonacci number */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    return A;
}
