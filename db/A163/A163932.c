/*
 * A163932  Triangle related to the asymptotic expansion of E(x,m=3,n).
 * https://oeis.org/A163932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 67, u64 n <= 191,
 *            u128 n <= 530, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A163932
    a(n) = binomial(m+1,2) * unsigned_stirling1(n+1,m+1)
    where the linear index n satisfies
      n = 1 + (row-1)*row/2 + (m-1)
    (row, m) are the row and column of the triangle entry.

    unsigned_stirling1(p,q) are the unsigned Stirling numbers of the first
    kind, defined by
      c(0,0)=1,  c(n,0)=0 (n>0)
      c(n,k)=c(n-1,k-1)+(n-1)*c(n-1,k)
*/

#include <stdint.h>
#include "../utils/binomial.h"          /* binomial(idx_t a, idx_t b) */

#define MAX_K 2000   /* safe upper bound for m+1 for the tested range */

static seq_t unsigned_stirling1(idx_t n, idx_t k)
{
    seq_t prev[MAX_K] = {0};
    seq_t cur[MAX_K]  = {0};
    if (k == 0)
        return (n == 0) ? (seq_t)1 : (seq_t)0;

    prev[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = 0;
        idx_t limit = (i < k) ? i : k;
        for (idx_t j = 1; j <= limit; ++j) {
            cur[j] = prev[j-1] + (seq_t)(i-1) * prev[j];
        }
        /* copy cur into prev for the next iteration */
        for (idx_t j = 0; j <= limit; ++j) {
            prev[j] = cur[j];
        }
    }
    return prev[k];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not defined for n<1, return 0 */

    /* Determine the row 'r' such that 1 + (r-1)*r/2 <= n < 1 + r*(r+1)/2. */
    idx_t r = 1;
    while ( (idx_t)1 + ((r+1) * r) / 2 <= n )
        ++r;

    /* offset of the start of this row */
    idx_t offset = (idx_t)1 + ((r-1) * r) / 2;

    /* column index m (1-based) */
    idx_t m = n - offset + 1;

    /* binomial part: binomial(m+1, 2) = (m+1)*m/2 */
    seq_t bin = binomial(m + 1, 2);

    /* unsigned Stirling number: c(r+1, m+1) */
    seq_t st = unsigned_stirling1(r + 1, m + 1);

    return bin * st;
}
