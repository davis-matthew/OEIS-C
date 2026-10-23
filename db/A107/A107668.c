/*
 * A107668  Column 0 of triangle A107667.
 * https://oeis.org/A107668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula implemented: a(n) = (n+1) * P(n+1,n+1)
   where P(n,1)=1, P(n,k)=P(n-1,k) + n*P(n,k-1) for 1<k<n,
   and P(n,n)=n*P(n,n-1).  See OEIS definition A107668.
   The algorithm builds the rows of P up to n+1 in O(n^2) arithmetic
   operations using two temporary arrays.
*/

#include <string.h>   /* for memset */

#ifndef MAX_A107668   /* maximum n supported, adjust if necessary */
#define MAX_A107668 10001
#endif

/* MAX_A107668 must be ≥ 10001 because n can be up to 10000. */

static seq_t P_prev[MAX_A107668];
static seq_t P_curr[MAX_A107668];

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* m = n+1, the last row needed */
    idx_t m = n + 1;
    if (m >= MAX_A107668) {
        /* out of preallocated bounds – the caller should not request >9999 */
        return (seq_t)0;
    }

    /* initialize first row: P(1,1)=1 */
    P_prev[1] = (seq_t)1;

    /* build rows 2 .. m */
    for (idx_t i = 2; i <= m; ++i) {
        P_curr[1] = (seq_t)1;          /* P(i,1)=1 */

        /* compute P(i,k) for 2 <= k <= i-1 */
        for (idx_t k = 2; k <= i - 1; ++k) {
            P_curr[k] = P_prev[k] + (seq_t)i * P_curr[k - 1];
        }

        /* compute diagonal element P(i,i)=i*P(i,i-1) */
        P_curr[i] = (seq_t)i * P_curr[i - 1];

        /* copy current row to previous for next iteration */
        for (idx_t k = 1; k <= i; ++k)
            P_prev[k] = P_curr[k];
    }

    /* after loop, P_curr[m] holds P(m,m) = Q(m) */
    seq_t result = (seq_t)(n + 1) * P_curr[m];
    return result;
}
