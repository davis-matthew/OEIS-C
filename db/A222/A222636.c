/*
 * A222636  Poly-Cauchy numbers c_n^(-3).
 * https://oeis.org/A222636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n,k) * (k+1)^3
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the poly‑Cauchy numbers c_n^(-3) as defined in OEIS A222636.
   Uses a standard O(n²) recurrence for Stirling numbers of the first kind.
   The array of Stirling numbers is allocated with malloc/free so no
   external utilities are required.  All arithmetic is performed in seq_t. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Allocate array for Stirling numbers of the current row. */
    size_t sz = (size_t)(n + 1);
    seq_t *S = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!S) return 0;               /* out of memory; return zero as a safe fallback */

    /* Initialise s(0,0)=1, all other entries zero. */
    for (size_t i = 0; i < sz; ++i) S[i] = 0;
    S[0] = 1;

    /* Build Stirling numbers up to row n. */
    for (idx_t m = 1; m <= n; ++m) {
        /* Update in reverse order to reuse the same array. */
        for (idx_t k = m; k >= 1; --k) {
            S[k] = S[k - 1] - (seq_t)(m - 1) * S[k];
        }
        S[0] = 0;   /* s(m,0)=0 for m>0 */
    }

    /* Accumulate the weighted sum a(n) = Σ s(n,k) * (k+1)^3. */
    seq_t ans = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t v = (seq_t)(k + 1);         /* (k+1) */
        v = v * v * v;                    /* (k+1)^3, using seq_t multiplication */
        ans += S[k] * v;
    }

    free(S);
    return ans;
}
