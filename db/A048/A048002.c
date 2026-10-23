/*
 * A048002  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= n/3.
 * https://oeis.org/A048002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, 5k ≤ floor(n/3) && k ≤ n – floor(n/3)}
 *            C(floor(n/3), 5k) · C(n – floor(n/3), k)
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts non‑empty subsets of {1,…,n} in which exactly 5/6 of the elements are ≤ n/3 */

static seq_t _binomial(idx_t N, idx_t K)
{
    /* Return C(N,K).  Works for both integer and arbitrary‑precision seq_t.  */
    if (K > N)               return 0;
    if (K == 0 || K == N)    return 1;

    /* Use symmetry to reduce the number of iterations. */
    if (K > N - K) K = N - K;

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        /* Multiply first to keep the result integral; division is exact. */
        res = res * (seq_t)(N - K + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* Offset begins at 1; safe guard. */

    idx_t m = n / 3;                     /* floor(n/3) – elements ≤ n/3           */
    idx_t r = n - m;                     /* elements > n/3                        */

    seq_t total = 0;

    /* For a subset the number of elements > n/3 must be k,
       and exactly 5k elements come from the ≤ n/3 part. */
    for (idx_t k = 1; 5 * k <= m && k <= r; ++k)
    {
        seq_t choose_left  = _binomial(m, 5 * k); /* C(m, 5k)   */
        seq_t choose_right = _binomial(r, k);    /* C(r, k)    */
        total += choose_left * choose_right;
    }

    return total;
}
