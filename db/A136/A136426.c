/*
 * A136426  Triangle T(n,0)=0 and T(n,k) = -A028421(n-1,k-1), 0<k<=n.
 * https://oeis.org/A136426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   The sequence is the row‑major enumeration of the triangle
 *            T(n,k), with T(n,0)=0 and for n≥1, k≥1 T(n,k) = -k * s(n,k),
 *            where s(n,k) is the unsigned Stirling number of the first kind
 *            (count of permutations of n elements with exactly k cycles).
 *            Thus a(n) = 0 if the corresponding entry lies in colum...
 * Range:     i8 n <= 23, i16 n <= 39, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required: seq_t and idx_t are provided by the build environment. */
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* offset 1: the first term a(1) corresponds to T(0,0). */
    if (n < 1)
        return 0;               /* undefined for n < 1, but safety */

    /* Find the row index 'row' such that
       row*(row+1)/2 < n <= (row+1)*(row+2)/2.  */
    idx_t row = (idx_t)ceil((sqrt(8.0 * (double)n + 1.0) - 3.0) / 2.0);

    /* Correct possible rounding errors with a small adjustment loop. */
    while (((unsigned __int128)(row + 1) * (row + 2) / 2) < n)
        ++row;
    while (((unsigned __int128)row * (row + 1) / 2) >= n)
        --row;

    /* Number of terms preceding this row. */
    idx_t prev_terms = (unsigned __int128)row * (row + 1) / 2;

    /* Column index within the row (0‑based). */
    idx_t k = n - prev_terms - 1;
    if (k == 0)
        return 0;               /* T(n,0) is always 0 */

    /* Compute the Stirling number of the first kind s(row, k)
       using the recurrence:
           s(0,0)=1, s(i,0)=0 for i>0
           s(i,j) = s(i-1,j-1) + (i-1)*s(i-1,j).        */
    size_t len  = (size_t)k + 1;   /* length of the working arrays */
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev || !curr) {         /* allocation failure */
        if (prev) free(prev);
        if (curr) free(curr);
        return 0;
    }
    prev[0] = 1;                  /* s(0,0)=1 */

    for (idx_t i = 1; i <= row; ++i) {
        curr[0] = 0;
        size_t maxj = (size_t)(i < k ? i : k);
        for (size_t j = 1; j <= maxj; ++j) {
            seq_t val = prev[j - 1];
            val += ((seq_t)(i - 1)) * prev[j];
            curr[j] = val;
        }
        /* swap arrays for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t s_val = prev[k];
    seq_t result = -((seq_t)k) * s_val;

    free(prev);
    free(curr);
    return result;
}
