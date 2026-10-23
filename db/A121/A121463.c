/*
 * A121463  Triangle read by rows: T(n,k) is the number of nondecreasing Dyck
 *          paths of semilength n, having k distinct valley levels (n>=1,
 *          k>=0).
 * https://oeis.org/A121463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A121463:  a(n) = T(r,k) where the row r is the
   smallest integer with Σ_{i=1}^{r} (1+⌊i/2⌋) ≥ n ,
   and k = n - Σ_{i=1}^{r-1} (1+⌊i/2⌋).
   For k=0  :  T(r,0)=1
   For k>0  :  T(r,k)= Σ_{j=0}^{r-2k} binomial(r,2k+j)*binomial(k-1+j,k-1)  */
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* sequence starts at n=1 */

    /* Find the row r that contains the n‑th element. */
    idx_t r = 1;
    idx_t cum = 1;                     /* number of elements up to current row */
    while (cum < n) {
        ++r;
        idx_t len = 1 + (r >> 1);       /* 1 + floor(r/2) */
        cum += len;
    }

    idx_t len   = 1 + (r >> 1);
    idx_t start = cum - len + 1;        /* first index of row r */
    idx_t k = n - start;                /* 0‑based column in row r */

    if (k == 0)
        return 1;                      /* T(r,0)=1 */

    seq_t sum = 0;
    idx_t maxj = r - 2 * k;            /* j runs from 0 to r-2k */

    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t term1 = binomial(r, 2 * k + j);
        seq_t term2 = binomial(k - 1 + j, k - 1);
        sum += term1 * term2;
    }

    return sum;
}
