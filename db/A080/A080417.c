/*
 * A080417  Signed Stirling numbers of the second kind.
 * https://oeis.org/A080417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the (flattened) triangle of signed Stirling numbers of
 *            the second kind. Recurrence: T(1,1)=1, T(n,k)= -T(n-1,k-1) + k *
 *            T(n-1,k) (n>1). The sequence a is obtained by listing the
 *            entries of T row by row (a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2),
 *            a(4)=T(3,1), …)
 * Range:     i8 n <= 23, i16 n <= 48, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;                    /* offset is 1; below can be undefined */

    /* Find the row r containing the nth element:
       minimal r such that r*(r+1)/2 >= n  */
    idx_t r = 1;
    while ((r * (r + 1)) / 2u < n)
        ++r;

    /* Column index within that row (1‑based) */
    idx_t k = n - (r * (r - 1)) / 2u;

    /* Base case: row 1 contains only a single entry 1 */
    if (r == 1u)
        return 1;

    /* Allocate storage for the previous row.
       We use (r+1) so that indices 1..r are available. */
    seq_t *prev = (seq_t*)calloc(r + 1, sizeof(seq_t));
    prev[1] = 1;                         /* T(1,1) */

    /* Build successive rows up to row r */
    for (idx_t row = 2u; row <= r; ++row) {
        seq_t *curr = (seq_t*)calloc(row + 1, sizeof(seq_t));
        for (idx_t pos = 1u; pos <= row; ++pos) {
            seq_t term1 = (pos > 1u) ? prev[pos - 1] : 0;          /* T(row-1, pos-1) */
            seq_t term2 = (pos <= row - 1u) ? prev[pos] : 0;       /* T(row-1, pos)  */
            curr[pos] = -term1 + (seq_t)pos * term2;
        }
        free(prev);
        prev = curr;
    }

    seq_t result = prev[k];
    free(prev);
    return result;
}
