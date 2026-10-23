/*
 * A012960  Expansion of e.g.f. exp(arctan(x)+log(x+1)).
 * https://oeis.org/A012960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(3)=2; For n>=4: a(n) = -(n-3)*a(n-1) -
 *            (n-1)*(n-3)*a(n-2) - (n-1)*(n-2)*(n-4)*a(n-3)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)2;

    /* previous three values: a_{i-3}, a_{i-2}, a_{i-1} */
    seq_t prev3 = (seq_t)2; /* a(1) */
    seq_t prev2 = (seq_t)3; /* a(2) */
    seq_t prev1 = (seq_t)2; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t c1 = (seq_t)(i - 3);                           /* (i-3)       */
        seq_t c2 = (seq_t)(i - 1) * (seq_t)(i - 3);           /* (i-1)*(i-3) */
        seq_t c3 = (seq_t)(i - 1) * (seq_t)(i - 2) * (seq_t)(i - 4); /* (i-1)*(i-2)*(i-4) */

        seq_t term1 = -c1 * prev1;           /* -(i-3)*a_{i-1} */
        seq_t term2 = -c2 * prev2;           /* -(i-1)*(i-3)*a_{i-2} */
        seq_t term3 = -c3 * prev3;           /* -(i-1)*(i-2)*(i-4)*a_{i-3} */

        seq_t cur = term1 + term2 + term3;   /* a(i) */

        /* shift previous values for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;   /* a(n) */
}
