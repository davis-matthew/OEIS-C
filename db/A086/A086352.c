/*
 * A086352  Main diagonal of square array A086350.
 * https://oeis.org/A086352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            ((1+sqrt(2))*(n+sqrt(2))^n-(1-sqrt(2))*(n-sqrt(2))^n)/sqrt(8)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the identity that a(n) is the sum of the two
 * coefficients of the expansion of (n+sqrt(2))^n, i.e. a(n) = P + Q
 * where (n+sqrt(2))^n = P + Q*sqrt(2).  The value is computed by
 * repeated multiplication: (a,b)*(c,d) = (ac+2bd , ad+bc).  The code
 * works for any seq_t type and only uses integer arithmetic. */

seq_t a(idx_t n)
{
    /* base case n = 0 : (0+sqrt2)^0 = 1 = 1 + 0*sqrt2 */
    if (n == (idx_t)0) return (seq_t)1;

    /* convert integer n to seq_t once */
    seq_t n_val = (seq_t)n;

    /* start with the first power: (n+sqrt2)^1 = n + 1*sqrt2 */
    seq_t P = n_val;   /* coefficient of 1    */
    seq_t Q = (seq_t)1;/* coefficient of sqrt2 */

    /* multiply (P + Q*sqrt2) by (n + sqrt2) one additional time
     * for each exponent step from 2 to n.  This takes O(n) steps. */
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t newP = P * n_val + Q * (seq_t)2;
        seq_t newQ = P + Q * n_val;
        P = newP;
        Q = newQ;
    }

    /* a(n) = P + Q */
    return P + Q;
}
