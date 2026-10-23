/*
 * A122542  Triangle T(n,k), 0 <= k <= n, read by rows, given by [0, 2, -1, 0,
 *          0, 0, 0, 0, ...] DELTA [1, 0, 0, 0, 0, 0, 0, 0, ...] where DELTA
 *          is the operator defined in A084938.
 * https://oeis.org/A122542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(r,k) of the triangle T defined by T(0,0)=1,
 *            T(1,0)=0, T(1,1)=1 and for n>1,
 *            T(n,k)=T(n-1,k)+T(n-1,k-1)+T(n-2,k-1). The sequence a(n) is
 *            obtained by reading the triangle row by row; i.e. a(n) = T(r,k)
 *            where r and k satisfy n = r*(r+1)/2 + k, 0 ≤ k ≤ r
 * Range:     u8 n <= 48, u16 n <= 127, u32 n <= 419, u64 n <= 1509,
 *            u128 n <= 5612, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r and column k such that
       n = r*(r+1)/2 + k, 0 ≤ k ≤ r */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        ++r;
    }
    idx_t k = n - cum;

    /* Size for a square array that is large enough to hold all
       entries up to row r (array indices 0 … r). */
    idx_t size = r + 1;

    /* Allocate a contiguous block for the triangle. */
    seq_t *data = (seq_t *)malloc((size_t)size * (size_t)size * sizeof(seq_t));
    if (!data) abort();

    /* Initialize the first row. */
    data[0] = (seq_t)1;          /* row 0, column 0 */

    /* Initialize the second row if it exists. */
    if (r >= 1) {
        data[1]          = (seq_t)0;                          /* row 0, column 1 (unused) */
        data[size]       = (seq_t)0;                          /* row 1, column 0 */
        data[size + 1]   = (seq_t)1;                          /* row 1, column 1 */
    }

    /* Compute rows 2 … r.  Each row i has length i+1 indices 0 … i. */
    for (idx_t i = 2; i <= r; ++i) {
        idx_t base_i   = i         * size;   /* start of row i   */
        idx_t base_ip1 = (i - 1)   * size;   /* start of row i-1 */
        idx_t base_ip2 = (i - 2)   * size;   /* start of row i-2 */

        for (idx_t j = 0; j <= i; ++j) {
            seq_t val = 0;

            /* T(i-1,j)  exists only if j ≤ i-1 */
            if (j <= i - 1)          val += data[base_ip1 + j];

            /* T(i-1,j-1) exists only if j ≥ 1 */
            if (j >= 1)              val += data[base_ip1 + j - 1];

            /* T(i-2,j-1) exists only if j ≥ 1 and j ≤ i-1 */
            if (j >= 1 && j <= i - 1) val += data[base_ip2 + j - 1];

            data[base_i + j] = val;
        }
    }

    /* Result is the desired entry. */
    seq_t result = data[r * size + k];

    free(data);
    return result;
}
