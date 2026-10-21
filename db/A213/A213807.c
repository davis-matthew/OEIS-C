/*
 * A213807  a(n) = Sum_{0<=i<j<k<=n} L(i)*L(j)*L(k), where L(m) is the m-th
 *          Lucas number A000032(m).
 * https://oeis.org/A213807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{0<=i<j<k<=n} L(i)*L(j)*L(k) (Lucas numbers)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t e1 = 0, e2 = 0, e3 = 0;          /* elementary symmetric sums */
    seq_t L_prev2 = 2, L_prev1 = 1;        /* L(0)=2, L(1)=1            */
    seq_t L_curr;

    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            L_curr = 2;
        } else if (i == 1) {
            L_curr = 1;
        } else {
            L_curr = L_prev2 + L_prev1;
            L_prev2 = L_prev1;
            L_prev1 = L_curr;
        }

        e3 = e3 + L_curr * e2;   /* update third symmetric sum */
        e2 = e2 + L_curr * e1;   /* update second symmetric sum */
        e1 = e1 + L_curr;        /* update first symmetric sum  */
    }

    return e3;
}
