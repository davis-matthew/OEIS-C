/*
 * A101927  E.g.f. of sin(arcsinh(x)) (odd powers only).
 * https://oeis.org/A101927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n+1) * ∏_{k=0}^{n-2} ((2*k+1)^2 + 1) (with a(1)=1,
 *            a(2)=-2). This follows from the product expression for |a(n)|
 *            and an alternating sign pattern. The recurrence a(n) =
 *            a(n-2)*((2*(n-3)+1)^2+1)*((2*(n-2)+1)^2+1) holds for n≥3
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-2;

    /* previous two terms: prev2 = a(i-2), prev1 = a(i-1)   */
    seq_t prev2 = (seq_t)1;      /* a(1)   */
    seq_t prev1 = (seq_t)-2;     /* a(2)   */

    for (idx_t i = 3; i <= n; ++i) {
        /* k1 = i-3, k2 = i-2 */
        idx_t k1 = i - 3;
        idx_t k2 = i - 2;

        /* compute (2*k+1)^2 + 1  as seq_t */
        seq_t p1 = (seq_t)(2*k1 + 1);
        p1 = p1 * p1 + (seq_t)1;

        seq_t p2 = (seq_t)(2*k2 + 1);
        p2 = p2 * p2 + (seq_t)1;

        /* a(i) = a(i-2) * p1 * p2  (sign handled automatically) */
        seq_t cur = prev2 * p1;
        cur = cur * p2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
