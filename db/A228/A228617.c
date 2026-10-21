/*
 * A228617  T(n,k) is the number of s in {1,...,n}^n having shortest run with
 *          the same value of length k; triangle T(n,k), n>=0, 0<=k<=n, read
 *          by rows.
 * https://oeis.org/A228617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the flattened triangle T(n,k) of OEIS
 *            A228617. T(n,k) = n * (W(n,k) - W(n,k+1)) where W(n,m) = Σ_{r}
 *            binom(n-1,r-1)*(n-1)^(r-1) over all compositions of n with parts
 *            ≥ m. This implementation uses a dynamic programming calculation
 *            of W(n,m)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 153,
 *            u128 n <= 378, big any n
 * Helpers:   count_T, w_min
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>     /* for memset */
#include "../utils/count_T.h"
#include "../utils/w_min.h"

/* @brief  Flattened triangle value a(n)
 * @param  n  index (offset 0)
 * @return   a(n) as seq_t
 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)            /* empty word */
        return (seq_t)1;

    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= n)
        ++r;

    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;                    /* column in the row */

    /* For k == 0 the shortest run cannot be 0 (except the empty row) */
    if (k == 0)
        return (seq_t)0;

    return count_T(r, k);
}
