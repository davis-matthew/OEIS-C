/*
 * A204855  G.f.: Sum_{n>=0} x^(n*(n+1)/2) / Product_{k=1..n} (1 -
 *          (n-k+1)*x^k).
 * https://oeis.org/A204855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 41, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204855
 * a(n) = coefficient of x^n in  
 *        Σ_{m≥0} x^(m*(m+1)/2) / ∏_{k=1}^{m} (1-(m-k+1)·x^k)   */
 
#include <stddef.h>          /* for size_t, idx_t is defined elsewhere */
 
/* A safe upper bound for the indices that will be requested in the tests.
   It is well above the 10000 bound mentioned in the problem statement. */
#define MAX_N 40000
 
/* Memoisation storage.  The arrays are static so that any class type used
   for seq_t is value‑initialised by the compiler. */
static seq_t seq[MAX_N + 1];
static seq_t dp[MAX_N + 1];
static idx_t seq_len = 0;      /* number of valid entries in seq[] (0 means none) */
 
/* Recompute all terms up to N (inclusive) */
static void compute_upto(idx_t N)
{
    /* Clear the answer array for the new values */
    for (idx_t i = 0; i <= N; ++i) seq[i] = 0;
 
    /* Iterate over all generators m such that their triangular offset is ≤ N */
    for (idx_t m = 0; ; ++m)
    {
        idx_t offset = (idx_t)((seq_t)m * ((seq_t)m + 1) / 2);
        if (offset > N) break;            /* no further m contributes */
 
        idx_t limit = N - offset;         /* maximal exponent for this term */
 
        /* Initialise the dynamic‑programming buffer */
        dp[0] = 1;
        for (idx_t i = 1; i <= limit; ++i) dp[i] = 0;
 
        /* Build the product ∏_{k=1..m} (1-(m-k+1)x^k)^{-1} */
        for (idx_t k = 1; k <= m; ++k)
        {
            seq_t weight = (seq_t)(m - k + 1);   /* coefficient of part k */
            for (idx_t e = k; e <= limit; ++e)
                dp[e] += weight * dp[e - k];
        }
 
        /* Accumulate the contribution of this m into the overall array */
        for (idx_t e = 0; e <= limit; ++e)
            seq[offset + e] += dp[e];
    }
 
    seq_len = N + 1;      /* now seq[0..N] contain valid results */
}
 
seq_t a(idx_t n)
{
    if (n >= seq_len) compute_upto(n);
    return seq[n];
}
