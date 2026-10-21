/*
 * A225477  Triangle read by rows, 3^k*s_3(n, k) where s_m(n, k) are the
 *          Stirling-Frobenius cycle numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (3*n-1)*T(n-1,k) + 3*T(n-1,k-1) with T(0,0)=1. a(n) is
 *            the triangle read by rows of T(n,k)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute a(n) for all n >= 0.  The function works independently of
    the value of seq_t and respects the build types described in the
    specification.  */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Number of elements preceding row r.  */
    idx_t prev_sum = r * (r + 1) / 2;
    idx_t col      = n - prev_sum;    /* column index within the row */

    /* A safe upper bound for the maximum row that can appear for n up
       to the intended limits (≈140 for n ≈ 10000).  */
    const idx_t MAX_R = 200;
    seq_t     prev[MAX_R];
    seq_t     curr[MAX_R];

    /* Base row.  */
    prev[0] = (seq_t)1;

    /* Build rows 1..r.  */
    for (idx_t i = 1; i <= r; ++i)
    {
        seq_t factor = (seq_t)(3 * i - 1);

        curr[0] = factor * prev[0];
        for (idx_t k = 1; k < i; ++k)
            curr[k] = factor * prev[k] + 3 * prev[k - 1];
        curr[i] = 3 * prev[i - 1];

        /* Copy current row into prev for the next iteration. */
        for (idx_t k = 0; k <= i; ++k)
            prev[k] = curr[k];
    }

    /* The desired element is in the computed row.  */
    return prev[col];
}
