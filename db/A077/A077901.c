/*
 * A077901  Expansion of (1-x)^(-1)/(1+x-x^2-2*x^3).
 * https://oeis.org/A077901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the series expansion of (1 -
 *            x)^(-1) / (1 + x - x^2 - 2*x^3) (OEIS A077901)
 * Range:     i8 n <= 21, i16 n <= 45, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two indices directly */
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */
    if (n == 1) return (seq_t)0;   /* a(1) = 0 */

    /* Base values for the recurrence of h_n:
       h_0 = 1
       h_1 = -h_0
       h_2 = -h_1 + h_0        (so h_2 = 2)                     */
    seq_t h0 = (seq_t)1;
    seq_t h1 = -h0;              /* -1 */
    seq_t h2 = -h1 + h0;         /* 2 */

    /* a(n) = sum_{k=0}^n h_k, so compute the prefix sum as we go. */
    seq_t pref = h0 + h1 + h2;    /* pref = a(2) = 2 */
    if (n == 2) return pref;     /* a(2) = 2 */

    /* Only three previous h-values are needed to generate the next one. */
    seq_t older = h0;   /* h_{n-3} */
    seq_t prev  = h1;   /* h_{n-2} */
    seq_t curr  = h2;   /* h_{n-1} */

    /* Compute h_n and the running sum for n >= 3. */
    for (idx_t i = 3; i <= n; ++i) {
        /* Recurrence: h_i = -h_{i-1} + h_{i-2} + 2*h_{i-3} */
        seq_t newh = -curr + prev + (seq_t)2 * older;
        pref += newh;          /* a(i) = a(i-1) + h_i */

        /* Shift the sliding window. */
        older = prev;
        prev  = curr;
        curr  = newh;
    }
    return pref;
}
