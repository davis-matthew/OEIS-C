/*
 * A075514  a(0)=1; for n > 0, a(n)=sum(binomial(n,k)*(binomial(n+k,k+1)^2)*
 *          binomial(n+k,k),k=0..n).
 * https://oeis.org/A075514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^n binomial(n,k) * binomial(n+k,k)^1 *
 *            binomial(n+k,k+1)^2 (OEIS A075514)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)0;

    /* Initial values for the recurrence */
    seq_t B  = (seq_t)1;          /* binomial(n,0)          */
    seq_t C  = (seq_t)1;          /* binomial(n+0,0)        */
    seq_t C1 = (seq_t)n;          /* binomial(n,1)          */

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        /* term = B * C * C1 * C1   (C1^2 = binomial(n+k,k+1)^2) */
        seq_t term = B * C;            /* B * C  */
        term = term * C1;              /* * C1   */
        term = term * C1;              /* * C1   */
        sum = sum + term;

        if (k == n) {
            break;   /* Next updates are unnecessary */
        }

        /* Update B  = binomial(n,k+1) = B * (n-k) / (k+1) */
        B = (B * (seq_t)(n - k)) / (seq_t)(k + (idx_t)1);

        /* Update C  = binomial(n+k+1,k+1) = C * (n+k+1) / (k+1) */
        C = (C * (seq_t)(n + k + 1)) / (seq_t)(k + (idx_t)1);

        /* Update C1 = binomial(n+k+1,k+2) =
           C1 * (n+k+1) / (k+2)                */
        C1 = (C1 * (seq_t)(n + k + 1)) / (seq_t)(k + (idx_t)2);
    }

    return sum;
}
