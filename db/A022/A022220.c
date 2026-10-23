/*
 * A022220  Gaussian binomial coefficients [ n,2 ] for q = 6.
 * https://oeis.org/A022220
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..2} (6^(n-i+1)-1)/(6^i-1) Recurrence
 *            (derivable from the generating function): a(n) = 43*a(n-1) -
 *            258*a(n-2) + 216*a(n-3) for n >= 5 with initial values a(2)=1,
 *            a(3)=43, a(4)=1591
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;               /* values below the offset are undefined */
    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)43;
    if (n == 4)
        return (seq_t)1591;

    /* Compute using the recurrence.  Use 5‑term sliding window. */
    seq_t a_prev3 = (seq_t)1;      /* a(2) */
    seq_t a_prev2 = (seq_t)43;     /* a(3) */
    seq_t a_prev1 = (seq_t)1591;   /* a(4) */
    seq_t current   = (seq_t)0;

    for (idx_t i = 5; i <= n; ++i) {
        current = (seq_t)43 * a_prev1;          /* 43*a(n-1) */
        current += (seq_t)216 * a_prev3;        /* +216*a(n-3) */
        current -= (seq_t)258 * a_prev2;        /* -258*a(n-2) */

        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = current;
    }
    return current;
}
