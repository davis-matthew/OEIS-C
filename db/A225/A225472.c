/*
 * A225472  Triangle read by rows, k!*S_3(n, k) where S_m(n, k) are the
 *          Stirling-Frobenius subset numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the triangle read by rows of T(n,k) = k!*S_3(n,k) where
 *            S_3 are Stirling-Frobenius subset numbers. T(n,k) satisfies the
 *            recurrence: T(n,k) = 3*k*T(n-1,k-1) + (2+3*k)*T(n-1,k), with
 *            T(0,0)=1, T(n,k)=0 if k>n. The index n of the OEIS sequence is
 *            obtained by reading T row by row. Thus a(0)=T...
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 48, u64 n <= 114, u128 n <= 336,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  External linkage as required by the build system.
    The function takes the OEIS index n (0‑based) and returns the
    corresponding term of sequence A225472.  */

#include <math.h>  /* for sqrt */

seq_t a(idx_t n)
{
    /* Find row r and column k corresponding to index n.
       The number of terms up to row r is (r+1)(r+2)/2, so r is the
       largest integer with r*(r+1)/2 <= n.  */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1) / 2);
    idx_t k = n - r * (r + 1) / 2;

    /* If k > r, something went wrong; return 0 for safety. */
    if (k > r)
        return 0;

    /* Compute T(r,k) using the recurrence.
       Since r <= ~140 for n <= 10000, a fixed small array suffices.  */
    if (r == 0 && k == 0)
        return 1;   /* base case */

    /* Use two arrays of size at least r+1 (here 201 to be safe). */
    static seq_t prev[201];
    static seq_t curr[201];

    /* Initialize the base row (n = 0).  */
    for (idx_t i = 0; i <= r; ++i)
        prev[i] = 0;
    prev[0] = 1;   /* T(0,0) = 1 */

    /* Iteratively build rows up to n = r.  */
    for (idx_t row = 1; row <= r; ++row) {
        idx_t max_j = (row <= k) ? row : k;   /* columns needed */

        /* Column 0 */
        curr[0] = 2 * prev[0];                /* (2+0)*prev[0] */

        /* Columns 1 .. max_j */
        for (idx_t j = 1; j <= max_j; ++j) {
            seq_t term1 = 3 * (seq_t)j * prev[j - 1];
            seq_t term2 = (2 + 3 * (seq_t)j) * prev[j];
            curr[j] = term1 + term2;
        }

        /* Prepare for next iteration: copy current row to prev.  */
        for (idx_t j = 0; j <= max_j; ++j)
            prev[j] = curr[j];
    }

    return prev[k];
}
