/*
 * A123149  Triangle T(n,k), 0<=k<=n, read by rows given by [1, 0, -1, 0, 0,
 *          0, 0, 0, ...] DELTA [0, 1, 0, -1, 0, 0, 0, 0, ...] where DELTA is
 *          the operator defined in A084938.
 * https://oeis.org/A123149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 335, u32 n <= 1099, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A123149 – triangle T(n,k) read row‑wise.
 * T(0,0)=1,  T(1,0)=1, T(1,1)=0,
 * for n≥2:
 *   if n even:  T(n,k)=T(n-1,k-1)+T(n-1,k)
 *   if n odd :  T(n,k)=T(n-1,k-1)+T(n-2,k)
 * T(n,k)=0 if k<0 or k>n.
 * The function below computes a(n), the sequence obtained by
 * traversing the triangle row‑by‑row.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* special small values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* find the row r containing the n‑th term:
       r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= (unsigned __int128)n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    idx_t r = hi;  /* row index */

    /* compute k, the position inside that row */
    unsigned __int128 base128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t base = (idx_t)base128;
    idx_t k = n - base;

    /* store previous two rows; we only need at most three rows */
    seq_t *prev2 = NULL;  /* row i-2 */
    seq_t *prev1 = NULL;  /* row i-1 */
    seq_t *curr  = NULL;  /* row i */

    /* row 0 */
    prev2 = (seq_t *)malloc(sizeof(seq_t));
    prev2[0] = (seq_t)1;

    /* row 1 */
    prev1 = (seq_t *)malloc(2 * sizeof(seq_t));
    prev1[0] = (seq_t)1;
    prev1[1] = (seq_t)0;

    /* if target is in row 0 or row 1 */
    if (r == 0) {
        /* already handled by n==0 */
        free(prev2);
        free(prev1);
        return (seq_t)1;
    }
    if (r == 1) {
        seq_t result = prev1[k];
        free(prev2);
        free(prev1);
        return result;
    }

    /* generate rows up to r */
    seq_t result = (seq_t)0;  /* will be set when i == r */
    for (idx_t i = 2; i <= r; ++i) {
        curr = (seq_t *)malloc((i + 1) * sizeof(seq_t));

        for (idx_t j = 0; j <= i; ++j) {
            seq_t left  = (j >= 1) ? prev1[j - 1] : (seq_t)0;
            seq_t right;
            if ((i & 1) == 0)      /* even n */
                right = (j <= i - 1) ? prev1[j] : (seq_t)0;
            else                    /* odd n */
                right = (j <= i - 2) ? prev2[j] : (seq_t)0;
            curr[j] = left + right;
        }

        if (i == r)            /* target row reached */
            result = curr[k];

        free(prev2);
        prev2 = prev1;
        prev1 = curr;
        curr  = NULL;
    }

    /* clean up */
    free(prev2);
    free(prev1);

    return result;
}
