/*
 * A171661  Triangle T(n,k) (n >= 0, 0 <= k <= n) read by rows: T(n,0) =
 *          T(n,1) = A000984(n); for n >= 2 and k >= 2, T(n,k) = T(n,k-1) -
 *          T(n-1,k-2).
 * https://oeis.org/A171661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A171661.
   The sequence is obtained by reading the triangle T(n,k) in row‑by‑row order.
   The triangle is defined recursively:
     T(n,0) = T(n,1) = central_binomial(n) = C(2n,n),
     for n >= 2 and k >= 2:  T(n,k) = T(n,k-1) - T(n-1,k-2).
   a(n) is the entry with index n in the flattened triangle. */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find row r and column k such that the n-th flat index is
       located at T(r,k).  The cumulative number of entries up through
       row r is (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Column within the row. */
    idx_t k = n - (r * (r + 1) / 2);

    /* Build the triangle from row 0 up to row r.
       Only two rows are kept at any time: the previous row and the
       current row.  Memory is freed after use. */
    seq_t *prev = NULL;
    seq_t *curr = NULL;

    for (idx_t i = 0; i <= r; ++i) {
        curr = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        for (idx_t j = 0; j <= i; ++j) {
            if (j == 0 || j == 1) {
                /* Central binomial coefficient C(2i,i). */
                curr[j] = binom(2 * i, i);
            } else {
                /* Recurrence: T(i,j) = T(i,j-1) - T(i-1,j-2). */
                curr[j] = curr[j - 1] - prev[j - 2];
            }
        }
        if (i == r) {
            seq_t result = curr[k];
            free(prev);
            free(curr);
            return result;
        }
        free(prev);
        prev = curr;
    }

    /* This point should never be reached. */
    return 0;
}
