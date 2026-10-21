/*
 * A210803  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210804; see the Formula section.
 * https://oeis.org/A210803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), where T is defined by T(1,0)=1, T(2,0)=T(2,1)=1,
 *            T(3,0)=1, T(3,1)=3, T(3,2)=2, and for n≥4, 0≤k<n
 *            T(n,k)=4*T(n-1,k)+T(n-1,k-1)−3*T(n-2,k)−2*T(n-2,k-1)+T(n-2,k-2),
 *            with T(n,k)=0 if k<0 or k≥n. The sequence a(n) lists the rows of
 *            this triangle consecutively (row 1: 1; row 2: 1,1; row...
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>   /* for placement new in C++ */
#endif

/* helper: compute the smallest r such that r(r+1)/2 ≥ n */
static inline idx_t row_of_n(idx_t n)
{
    /* binary search on r in [1, n] */
    idx_t low = 1, high = n, mid, val;
    while (low < high) {
        mid = low + (high - low) / 2;
        /* check mid*(mid+1)/2 >= n avoiding overflow */
        val = mid;
        val = val * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1 */

    idx_t r = row_of_n(n);           /* row containing nth term */
    idx_t col = n - (idx_t)((r - 1) * r / 2) - 1;  /* 0‑based column */

    /* handle first three rows directly */
    if (r == 1) return 1;
    if (r == 2) return 1;           /* both columns are 1 */
    /* for r ≥ 3 we build rows iteratively, keeping only last two */
    seq_t *prev2 = NULL;   /* row n-2 */
    seq_t *prev1 = NULL;   /* row n-1 */

    /* allocate row 1 */
#ifdef __cplusplus
    prev2 = new seq_t[1];
    prev2[0] = 1;
#else
    prev2 = (seq_t*)malloc(sizeof(seq_t));
    if (!prev2) abort();
    prev2[0] = 1;
#endif

    /* allocate row 2 */
#ifdef __cplusplus
    prev1 = new seq_t[2];
    prev1[0] = 1;
    prev1[1] = 1;
#else
    prev1 = (seq_t*)malloc(2 * sizeof(seq_t));
    if (!prev1) abort();
    prev1[0] = 1;
    prev1[1] = 1;
#endif

    seq_t *curr = NULL;

    for (idx_t cur = 3; cur <= r; ++cur) {
        size_t len = (size_t)cur;
#ifdef __cplusplus
        curr = new seq_t[len];
#else
        curr = (seq_t*)malloc(len * sizeof(seq_t));
        if (!curr) abort();
#endif
        for (size_t k = 0; k < len; ++k) {
            seq_t a0 = (k < cur - 1) ? prev1[k] : 0;                     /* T(cur-1,k)   */
            seq_t b0 = (k > 0)       ? prev1[k - 1] : 0;                 /* T(cur-1,k-1) */
            seq_t c0 = (k < cur - 2) ? prev2[k] : 0;                     /* T(cur-2,k)   */
            seq_t d0 = (k > 0 && k - 1 < cur - 2) ? prev2[k - 1] : 0;    /* T(cur-2,k-1) */
            seq_t e0 = (k >= 2 && k - 2 < cur - 2) ? prev2[k - 2] : 0;    /* T(cur-2,k-2) */

            /* all operations are on unsigned types, values are non‑negative */
            seq_t val = 4 * a0 + b0 - 3 * c0 - 2 * d0 + e0;
            curr[k] = val;
        }
        /* free the row that is no longer needed */
#ifdef __cplusplus
        delete[] prev2;
#else
        free(prev2);
#endif
        prev2 = prev1;
        prev1 = curr;
    }

    seq_t result = prev1[(size_t)col];
#ifdef __cplusplus
    delete[] prev2;
    delete[] prev1;
#else
    free(prev2);
    free(prev1);
#endif
    return result;
}
