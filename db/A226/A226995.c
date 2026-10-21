/*
 * A226995  Number of lattice paths from (0,0) to (n,n) consisting of steps
 *          U=(1,1), H=(1,0) and S=(0,1) such that the first step leaving the
 *          diagonal (if any) is an H step and the last step joining the
 *          diagonal (if any) is a S step.
 * https://oeis.org/A226995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1..n} C(2k-2, k-1) * C(n+k, n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(a,b) is the binomial coefficient. The summand counts the
   paths with k horizontal and k vertical steps (labels H and S)
   whose first non‑U step is H and whose last join to the diagonal
   is via a S step.  If k = 0 the unique all‑U path is counted.   */

/* The function below computes a(n) efficiently for any n
   using only integer arithmetic.  It works for 64‑bit,
   128‑bit or arbitrary precision seq_t types.        */
seq_t a(idx_t n)
{
    seq_t sum = 1;       /* term for k = 0 (all U's)          */
    seq_t c   = 1;       /* C(0,0) = 1  ==> C(2k-2, k-1) when k=1 */
    seq_t f   = 1;       /* f(0) = C(n, n) = 1                */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* f(k) = C(n+k, n-k) = C(n+k, 2k)  (recurrence) */
        seq_t num_f = f * (seq_t)(n + k) * (seq_t)(n - k + 1);
        seq_t den_f = (seq_t)(2 * k - 1) * (seq_t)(2 * k);
        f = num_f / den_f;

        /* accumulate term C(2k-2,k-1) * f(k) */
        sum += c * f;

        /* update C(2k, k) for next iteration (k → k+1) */
        if (k < n)
        {
            seq_t num_c = c * (seq_t)(2 * k) * (seq_t)(2 * k - 1);
            seq_t den_c = (seq_t)k * k;
            c = num_c / den_c;
        }
    }

    return sum;
}
