/*
 * A206474  Riordan array ((1+x-x^2)/(1-x^2), x/(1-x^2)).
 * https://oeis.org/A206474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = the n-th element of the triangle T(n,k) defined by
 *            T(0,0)=1, T(1,0)=T(1,1)=1, T(2,0)=0, T(2,1)=T(2,2)=1, and for n
 *            ≥ 3, k ≥ 0 T(n,k) = T(n-1,k-1) + T(n-2,k) (with T(n,k)=0 if k<0
 *            or k>n). The sequence a(n) is the triangle read by rows
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that n lies in row r */
    idx_t r = 0;
    /* cumulative entries up to and including row r */
    idx_t cum = (r + 1) * (r + 2) / 2;
    while (cum <= n) {
        ++r;
        cum = (r + 1) * (r + 2) / 2;
    }
    /* Now `cum` > n, so row = r and cum_before = r*(r+1)/2 */
    idx_t cum_before = r * (r + 1) / 2;
    idx_t col = n - cum_before;

    /* Handle the first three rows directly */
    if (r == 0) {
        return (seq_t)1;           /* T(0,0) */
    }
    if (r == 1) {
        /* row 1: 1, 1 */
        return (seq_t)1;
    }
    if (r == 2) {
        /* row 2: 0, 1, 1 */
        return (col == 0) ? (seq_t)0 :
               /* col==1 or 2 */
               (seq_t)1;
    }

    /* Dynamic construction for rows 3..r.  Keep only two previous rows:
       prev2 = row (current-2), prev1 = row (current-1).  */
    /* Initialize prev2 as row 1: [1,1] */
    seq_t *prev2 = (seq_t *)malloc(sizeof(seq_t) * 2);
    if (!prev2) return (seq_t)0; /* allocation failure (unlikely) */
    prev2[0] = (seq_t)1;
    prev2[1] = (seq_t)1;

    /* Initialize prev1 as row 2: [0,1,1] */
    seq_t *prev1 = (seq_t *)malloc(sizeof(seq_t) * 3);
    if (!prev1) {
        free(prev2);
        return (seq_t)0;
    }
    prev1[0] = (seq_t)0;
    prev1[1] = (seq_t)1;
    prev1[2] = (seq_t)1;

    /* Compute rows 3 .. r */
    for (idx_t row = 3; row <= r; ++row) {
        seq_t *cur = (seq_t *)malloc(sizeof(seq_t) * (row + 1));
        if (!cur) {
            /* clean up and exit on allocation failure */
            free(prev1);
            free(prev2);
            return (seq_t)0;
        }
        for (idx_t k = 0; k <= row; ++k) {
            seq_t a = (k > 0) ? prev1[k - 1] : (seq_t)0;
            seq_t b = (k <= row - 2) ? prev2[k] : (seq_t)0;
            cur[k] = a + b;
        }
        free(prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    seq_t result = prev1[col];
    free(prev1);
    free(prev2);
    return result;
}
