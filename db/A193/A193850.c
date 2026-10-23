/*
 * A193850  Triangular array: the fission of ((x+2)^n) by (q(n,x)) given by
 *          q(n,x)=x^n+x^(n-1)+...+x+1.
 * https://oeis.org/A193850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A193850:  triangular array defined by
 *      a[0][0] = 2;
 *      for r ≥ 1, 0 ≤ k < r:
 *          a[r][k] = 2*a[r-1][k] + (k>0 ? a[r-1][k-1] : 0);
 *      a[r][r] = 3*a[r-1][r-1] + 2;
 *  The sequence a(n) is obtained by reading the array in row‑major order:
 *      a(0) = a[0][0],
 *      a(1) = a[1][0], a(2) = a[1][1],
 *      a(3) = a[2][0], a(4) = a[2][1], a(5) = a[2][2],
 *      …
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find the row r containing the n-th term.
       the first index of row r is T(r) = r*(r+1)/2          */
    idx_t r = 0;
    idx_t lo = 0, hi = 1;
    while ((hi * (hi + 1)) / 2 <= n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    r = lo;

    idx_t offset = (r * (r + 1)) / 2;
    idx_t k = n - offset;   /* column index inside the row   */

    /* If we are on row 0, the answer is constant 2.           */
    if (r == 0)
        return (seq_t)2;

    /* Allocate two working arrays of length r+1.  The array
     * "prev" holds the values of the previous row; "curr"
     * will be built from it.                              */
    size_t len = (size_t)(r + 1);
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return 0; /* memory allocation failure – unlikely in test cases */
    }

    /* base row r = 0 */
    prev[0] = (seq_t)2;

    /* build rows 1 … r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)2 * prev[0];
        for (idx_t j = 1; j < i; ++j) {
            curr[j] = (seq_t)2 * prev[j] + prev[j - 1];
        }
        curr[i] = (seq_t)3 * prev[i - 1] + 2;

        /* swap arrays for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];
    free(prev);
    free(curr);
    return result;
}
