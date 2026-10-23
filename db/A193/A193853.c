/*
 * A193853  Triangular array: (1/2)*A193851.
 * https://oeis.org/A193853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^k * B(r,k) where B(0,0) = 1; B(r,0) = 3*B(r-1,0) + 1;
 *            B(r,k) = B(r-1,k-1) + B(r-1,k) (k ≥ 1). Here r and k are the row
 *            and column of the triangular array entry corresponding to the
 *            linear index n
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Determine the row r that contains index n */
    idx_t r = 0;
    idx_t start = 0;
    while (1) {
        idx_t next_start = start + (r + 1);
        if (n < next_start)
            break;
        start = next_start;
        ++r;
    }
    idx_t k = n - start;          /* column within the row */

    /* Allocate space for two successive rows */
    size_t max_len = (size_t)(r + 1);
    seq_t *prev = (seq_t *)malloc(max_len * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc(max_len * sizeof(seq_t));
    if (!prev || !cur) {          /* allocation failed – return 0 */
        seq_t zero = 0;
        if (prev) free(prev);
        if (cur) free(cur);
        return zero;
    }

    size_t prev_len = 1;
    prev[0] = (seq_t)1;          /* B(0,0) = 1 */

    /* Build rows up to r */
    for (size_t row = 1; row <= (size_t)r; ++row) {
        size_t cur_len = row + 1;

        /* First element of the row */
        cur[0] = (prev[0] * (seq_t)3) + (seq_t)1;

        /* Remaining elements via Pascal‑like addition */
        for (size_t j = 1; j < cur_len; ++j) {
            seq_t val = prev[j - 1];
            if (j < prev_len)
                val += prev[j];
            cur[j] = val;
        }

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
        prev_len = cur_len;
    }

    /* Compute the desired value: B(r,k) * 2^k */
    seq_t result = prev[k];
    result *= pow2(k);

    /* Free allocated memory */
    free(prev);
    free(cur);

    return result;
}
