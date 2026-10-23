/*
 * A192958  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 6*Fibonacci(n+1) - (2*n+5)
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 41, i64 n <= 87, i128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = -1, a(2) = 3, ...  (OEIS A192958) */

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n+1) iteratively.
       F(0)=0, F(1)=1, but we want F(n+1). */
    seq_t a0 = 0, a1 = 1;               /* a0 = F(k), a1 = F(k+1)   */
    idx_t i;
    for (i = 0; i < n + 1; ++i) {       /* run n+1 steps to reach F(n+1) */
        seq_t tmp = a0 + a1;            /* next Fibonacci number   */
        a0 = a1;
        a1 = tmp;
    }
    seq_t fn = a0;                      /* fn = F(n+1)              */

    seq_t result = 6 * fn;              /* 6*Fibonacci(n+1)         */
    seq_t linear = (seq_t)n * 2 + 5;    /* 2*n + 5 expressed in seq_t */
    result -= linear;                   /* subtract linear term     */
    return result;
}
