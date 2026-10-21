/*
 * A225479  Triangle read by rows, the ordered Stirling cycle numbers, T(n, k)
 *          = k!* s(n, k); n >= 0 k >= 0.
 * https://oeis.org/A225479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k satisfy r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2, and T(n,k) = k! * s(n,k) with s(n,k) being the
 *            unsigned Stirling cycle numbers (number of permutations of n
 *            elements with k cycles)
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 82, u64 n <= 197,
 *            u128 n <= 510, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Determine the row r that contains the n-th element. */
    double root = sqrt(1.0 + 8.0 * (double)n);
    idx_t r = (idx_t)((-1.0 + root) / 2.0);            /* floor(( -1 + √(1+8n))/2 ) */
    /* Compute the offset within the row (0‑based). */
    idx_t offset = n - r * (r + 1) / 2;

    /* Allocate space for Stirling numbers s(n,k).  The array holds
       one row of the Stirling table. */
    seq_t *s = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!s) return 0;                                 /* allocation failure */

    /* Initial row s(0,0) = 1.  All other entries are 0. */
    s[0] = 1;
    for (idx_t i = 1; i <= r; ++i) s[i] = 0;

    /* Build successive rows up to row r using the recurrence
       s(n,k) = s(n-1,k-1) + (n-1)*s(n-1,k).  The update is done
       backwards in k to reuse the same array. */
    for (idx_t nrow = 1; nrow <= r; ++nrow) {
        for (idx_t k = nrow; k >= 1; --k) {
            seq_t old = s[k];
            s[k] = s[k - 1] + ((seq_t)(nrow - 1) * old);
        }
        s[0] = 0;
    }

    /* Compute k! for k = offset. */
    seq_t kfact = 1;
    for (idx_t i = 2; i <= offset; ++i) {
        kfact *= i;
    }

    /* Result is k! * s(r,offset). */
    seq_t result = kfact * s[offset];

    free(s);
    return result;
}
