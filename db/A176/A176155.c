/*
 * A176155  Regular triangle, T(n, k) = f(n, k) - f(n, 0) + 1, where f(n, k) =
 *          Sum_{j=0..k} StirlingS1(n, n-j)*binomial(n,j) + Sum_{j=0..n-k}
 *          StirlingS1(n, n-j)*binomial(n, j), read by rows.
 * https://oeis.org/A176155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 37, i32 n <= 80, i64 n <= 193,
 *            i128 n <= 501, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A176155: Regular triangle
 *
 *  Definition (OEIS notation):
 *
 *      f(n,k) = sum_{j=0..k} S(n, n-j) * C(n,j)
 *                 + sum_{j=0..n-k} S(n, n-j) * C(n,j)
 *
 *      T(n,k) = f(n,k) - f(n,0) + 1
 *
 *  The flattened triangle a(n) is produced by reading T(r,k)
 *  row‑wise, with r starting at 0 and k running from 0 to r.
 *
 *  Here we compute a(n) directly for the requested index n.
 *  The algorithm:
 *
 *      1. Locate the row r that contains the index n
 *         using the triangular numbering:
 *             start_r = r*(r+1)/2         (index of first element in row r)
 *             end_r   = (r+1)*(r+2)/2 - 1 (index of last element in row r)
 *         Substituting n into this gives r and k = n - start_r.
 *
 *      2. Compute the signed Stirling numbers of the first kind
 *         S(n,k) for all k=0..n using the recurrence
 *             S(0,0)=1,   S(n,0)=0 (n>0)
 *             S(n,k)=S(n-1,k-1)-(n-1)*S(n-1,k)
 *         The recurrence is applied iteratively; only an array of
 *         length n+1 is needed.
 *
 *      3. For each j=0..n compute
 *             A[j] = S(n, n-j) * C(n,j)
 *         where C(n,j) is the binomial coefficient computed
 *         multiplicatively:
 *             C(n,0)=1;   C(n,j)=C(n,j-1)*(n-j+1)/j
 *
 *      4. Compute prefix sums of A[j]:
 *             P[0] = A[0];
 *             P[j] = P[j-1]+A[j]
 *
 *      5. For kth element of the row:
 *             f(n,k)   = P[k] + P[n-k]
 *             f(n,0)   = P[0] + P[n]
 *             T(n,k)   = f(n,k) - f(n,0) + 1
 *
 *      6. Return T(n,k) as a(idx_t n).
 *
 *  Complexity: O(r^2) time, O(r) space, where r ≈ sqrt(2n).
 *  For n≈10⁴ this is ≤ 150×150 operations, well below the
 *  2‑second limit. The routine is thread‑safe, has no global
 *  mutable state and works for any build of seq_t/idx_t.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that n is in [r(r+1)/2 , (r+1)(r+2)/2 - 1] */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;                      /* column index within the row */

    /* Allocate arrays of size r+1 (≤ 200 for n ≤ 10⁴) */
    seq_t *s   = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));   /* Stirling numbers */
    seq_t *aarr = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));   /* A[j] = S(r,r-j)*C(r,j) */
    seq_t *pref = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));   /* prefix sums of aarr */

    if (!s || !aarr || !pref) {
        /* Allocation failure – return 0 (unlikely for small r) */
        free(s); free(aarr); free(pref);
        return (seq_t)0;
    }

    /* Initialize Stirling numbers: s[0] = 1, others 0 */
    s[0] = (seq_t)1;

    /* Build row r of Stirling numbers of the first kind */
    for (idx_t i = 1; i <= r; ++i) {
        /* Update from the previous row in reverse to avoid overwriting data */
        for (idx_t j = i; j >= 1; --j) {
            seq_t left  = (j - 1 <= r) ? s[j - 1] : (seq_t)0;
            seq_t right = (j <= r) ? s[j] : (seq_t)0;
            s[j] = left - (seq_t)(i - 1) * right;
        }
        s[0] = (seq_t)0;
    }

    /* Compute binomial coefficients and A[j] = S(r, r-j) * C(r,j) */
    seq_t bin = (seq_t)1;                     /* C(r,0) */
    for (idx_t j = 0; j <= r; ++j) {
        aarr[j] = s[r - j] * bin;
        if (j < r) {
            seq_t mult = (seq_t)(r - j);
            bin = bin * mult / ((seq_t)(j + 1));   /* C(r,j+1) */
        }
    }

    /* Compute prefix sums of aarr */
    for (idx_t j = 0; j <= r; ++j) {
        if (j == 0) {
            pref[j] = aarr[0];
        } else {
            pref[j] = pref[j - 1] + aarr[j];
        }
    }

    /* f(r,k) = prefix[k] + prefix[r-k];  f(r,0) = prefix[0] + prefix[r] */
    seq_t f0   = pref[0] + pref[r];
    seq_t val  = pref[k] + pref[r - k] - pref[0] - pref[r] + (seq_t)1;

    /* Clean up */
    free(s);
    free(aarr);
    free(pref);

    return val;
}
