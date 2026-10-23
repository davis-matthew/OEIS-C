/*
 * A192925  Coefficient of x in the reduction by (x^2->x+1) of the polynomial
 *          p(n,x) defined below at Comments.
 * https://oeis.org/A192925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let A_n, B_n be the coefficients of x and 1 in the reduction
 *            (x^2 → x+1) of p(n,x). The following recurrences hold: A_0 = 0,
 *            B_0 = 1 A_1 = 1, B_1 = 0 For n ≥ 2: A_n = n*A_{n-1} + 2*A_{n-2}
 *            + B_{n-2} B_n = n*B_{n-1} + A_{n-2} + B_{n-2} The desired
 *            sequence a(n) is A_n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_prev2 = (seq_t)0;   /* A_0 */
    seq_t b_prev2 = (seq_t)1;   /* B_0 */
    seq_t a_prev1 = (seq_t)1;   /* A_1 */
    seq_t b_prev1 = (seq_t)0;   /* B_1 */

    seq_t a_cur, b_cur;
    for (idx_t i = 2; i <= n; ++i) {
        a_cur = (seq_t)i * a_prev1 + (seq_t)2 * a_prev2 + b_prev2;
        b_cur = (seq_t)i * b_prev1 + a_prev2 + b_prev2;
        a_prev2 = a_prev1; a_prev1 = a_cur;
        b_prev2 = b_prev1; b_prev1 = b_cur;
    }
    return a_prev1;
}
