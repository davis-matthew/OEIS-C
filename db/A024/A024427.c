/*
 * A024427  S(n,1) + S(n-1,2) + S(n-2,3) + ... + S(n+1-k,k), where k =
 *          floor((n+1)/2) and S(i,j) are Stirling numbers of the second kind.
 * https://oeis.org/A024427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0..n} S(n+1-i, i) where S(m,k) is the Stirling
 *            number of the second kind
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 32, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first term in the OEIS entry. */
    if (n == 0) return (seq_t)0;           /* safe guard, not used in OEIS */

    idx_t m_max = n + 1;                    /* maximum row index needed */
    size_t len  = (size_t)m_max + 1;        /* array size for the two rows */

    seq_t *row_prev = (seq_t*)calloc(len, sizeof(seq_t));  /* zeroed row 0 */
    seq_t *row_cur  = (seq_t*)calloc(len, sizeof(seq_t));
    if (!row_prev || !row_cur) {
        free(row_prev);
        free(row_cur);
        return (seq_t)0;
    }

    row_prev[0] = (seq_t)1;                 /* S(0,0) = 1 */

    seq_t result = 0;

    for (idx_t m = 1; m <= m_max; ++m) {
        /* zero out the current row before writing to it */
        for (size_t i = 0; i < len; ++i) row_cur[i] = 0;

        /* compute S(m,k) for k = 1 .. m */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t part1 = (k <= m - 1) ? row_prev[k] * (seq_t)k : 0;
            seq_t part2 = row_prev[k - 1];
            row_cur[k] = part1 + part2;    /* S(m,k) = k*S(m-1,k) + S(m-1,k-1) */
        }

        /* add the contribution S(m, n-m+1) if it exists */
        if (n >= m) {
            idx_t k_target = n - m + 1;
            if (k_target <= m) {
                result += row_cur[k_target];
            }
        }

        /* swap the two row buffers for the next iteration */
        seq_t *tmp = row_prev;
        row_prev = row_cur;
        row_cur  = tmp;
    }

    free(row_prev);
    free(row_cur);

    return result;
}
