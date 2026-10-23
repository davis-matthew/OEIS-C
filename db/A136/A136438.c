/*
 * A136438  Hypertribonacci number array read by antidiagonals.
 * https://oeis.org/A136438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is read from the hypertribonacci table by antidiagonals.
 *            Let T(i) be the tribonacci numbers (T(0)=0,T(1)=0,T(2)=1).
 *            Define a(1,i)=T(i) and for k>1 a(k,i)=∑_{j=0}^{i} a(k-1,j). The
 *            sequence a(n) is obtained by listing the entries a(k,i) with k+i
 *            equal to 1,2,3,..., traversing each anti‑diagon...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the offset 1: a(1) is the first term 0 */
    if (n == 0)
        return 0;

    /* Find the diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        ++d;

    /* offset within that anti‑diagonal (0‑based) */
    idx_t offset = n - (d - 1) * d / 2 - 1;
    /* the row we need: k = d - offset */
    idx_t target_k = d - offset;

    /* maximum index needed in the column, i.e. n = offset */
    idx_t m = offset;
    size_t sz = (size_t)(m + 1);

    /* allocate two rows of size m+1 */
    seq_t *row1 = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *row2 = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!row1 || !row2) {
        /* allocation failure – return 0 as safety */
        free(row1);
        free(row2);
        return 0;
    }

    /* row1 is the tribonacci row a(1,i) */
    if (m >= 0) row1[0] = 0;
    if (m >= 1) row1[1] = 0;
    if (m >= 2) row1[2] = 1;
    for (idx_t i = 3; i <= m; ++i)
        row1[i] = row1[i - 1] + row1[i - 2] + row1[i - 3];

    /* If we need the first row, we are done */
    if (target_k == 1) {
        seq_t result = row1[offset];
        free(row1);
        free(row2);
        return result;
    }

    /* Compute successive partial sums up to target_k */
    for (idx_t k = 2; k <= target_k; ++k) {
        seq_t running = 0;
        for (size_t i = 0; i < sz; ++i) {
            running += row1[i];
            row2[i] = running;
        }
        /* swap row1 and row2 for next iteration */
        seq_t *tmp = row1;
        row1 = row2;
        row2 = tmp;
    }

    seq_t result = row1[offset];
    free(row1);
    free(row2);
    return result;
}
