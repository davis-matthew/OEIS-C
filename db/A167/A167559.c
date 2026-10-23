/*
 * A167559  The row sums of triangle A167557.
 * https://oeis.org/A167559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{m=1..n} 4^(m-1)*(m-1)!*(n+m-2)!/(2*m-2)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term (1-indexed) of sequence A167559 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for offset 1 */

    seq_t sum   = 0;                 /* accumulated result       */
    seq_t pow4  = 1;                 /* 4^(m-1)                  */
    seq_t fact  = 1;                 /* (m-1)!                    */
    seq_t P     = 1;                 /* (n+m-2)!/(2*m-2)! initially for m=1 */

    /* compute (n-1)! to initialise P for m = 1   */
    for (idx_t k = 2; k < n; ++k)
        P *= (seq_t)k;

    for (idx_t m = 1; m <= n; ++m)
    {
        /* term = pow4 * fact * P */
        seq_t term = pow4 * fact;
        term *= P;
        sum += term;

        if (m == n) break;           /* last term processed */

        /* update helpers for next m */
        fact *= (seq_t)m;            /* (m)! → (m+1-1)!       */
        pow4 *= 4;                   /* 4^(m)                 */

        /* P_{m+1} = P_m * (n+m-1) / ((2*m)*(2*m-1)) */
        seq_t num = P * (seq_t)(n + m - 1);
        seq_t den = (seq_t)(2 * m) * (seq_t)(2 * m - 1);
        P = num / den;
    }

    return sum;
}
