/*
 * A223899  Poly-Cauchy numbers of the second kind hat c_n^(-2).
 * https://oeis.org/A223899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..n} (-1)^k * (k+1)^2 * Stirling1(n,k) (computed
 *            via |S1(n+1,k+1)| for k=0,1,2)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute |S1(n+1,1)|, |S1(n+1,2)|, |S1(n+1,3)| by recurrence.
       Use two rows of length 4 (k=0..3); k>3 are not needed.   */
    idx_t m = n + 1;            /* we need the (n+1)-st row        */
    seq_t prev[4] = { 0 };      /* values for row i-1             */
    seq_t curr[4] = { 0 };      /* values for row i               */

    /* base row for i=0: S1(0,0)=1, others 0 */
    prev[0] = 1;

    for (idx_t i = 1; i <= m; ++i) {
        curr[0] = 0;
        /* k = 1 */
        curr[1] = prev[0] - ((seq_t)(i - 1)) * prev[1];
        /* k = 2 */
        curr[2] = prev[1] - ((seq_t)(i - 1)) * prev[2];
        /* k = 3 */
        curr[3] = prev[2] - ((seq_t)(i - 1)) * prev[3];

        /* copy curr to prev for next iteration */
        prev[0] = curr[0];
        prev[1] = curr[1];
        prev[2] = curr[2];
        prev[3] = curr[3];
    }

    /* extract the needed Stirling numbers of the first kind */
    seq_t d1 = prev[1];   /* S1(n+1,1)  */
    seq_t d2 = prev[2];   /* S1(n+1,2)  */
    seq_t d3 = prev[3];   /* S1(n+1,3)  */

    /* absolute values (S1 can be negative) */
    seq_t abs1 = d1 >= 0 ? d1 : -d1;
    seq_t abs2 = d2 >= 0 ? d2 : -d2;
    seq_t abs3 = d3 >= 0 ? d3 : -d3;

    /* sum of terms: |S1|1 + 3|S1|2 + 2|S1|3 */
    seq_t sum = abs1 + (seq_t)3 * abs2 + (seq_t)2 * abs3;

    /* apply the final sign factor (-1)^n */
    return (n & 1) ? -sum : sum;
}
