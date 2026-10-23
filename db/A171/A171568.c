/*
 * A171568  Riordan array (f(x), x*f(x)) where f(x) is the g.f. of A064613.
 * https://oeis.org/A171568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened sequence of the Riordan array with
 *            generating function (f(x), x*f(x)), where f(x) is the g.f. of
 *            A064613. The triangle T(n,k) satisfies T(n,0)-T(n,1)=2^n T(n,k)
 *            = T(n-1,k-1)+3*T(n-1,k)+∑_{i=0..n} T(n-1,k+1+i) (k≥1) The
 *            function builds the triangle row by row, accumulating te...
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

idx_t row_for(idx_t n)
{
    idx_t r = 0;
    while (1) {
        __uint128_t tri = (__uint128_t)(r + 1) * (r + 2) / 2;
        if ((idx_t)tri <= n)
            r++;
        else
            break;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* static state: triangle row, flat vector, sizes */
    static seq_t *prev_row = NULL;
    static size_t prev_len = 0;   /* number of elements in prev_row */
    static size_t current_row = 0;/* row index of prev_row */

    static seq_t *flat = NULL;
    static size_t flat_len = 0;

    /* initialize on first call */
    if (prev_row == NULL) {
        prev_len = 1;
        prev_row = (seq_t*)malloc(prev_len * sizeof(seq_t));
        prev_row[0] = 1;                /* row 0: [1] */
        current_row = 0;

        flat_len = 1;
        flat = (seq_t*)malloc(flat_len * sizeof(seq_t));
        flat[0] = 1;
    }

    /* find row needed for index n */
    idx_t target_row = row_for(n);

    /* generate rows up to target_row */
    while (current_row < target_row) {
        /* compute next row (index current_row+1) */
        size_t n_prev = prev_len;          /* n_prev = current_row + 1 */
        size_t new_len = n_prev + 1;       /* length of new row */

        seq_t *new_row = (seq_t*)malloc(new_len * sizeof(seq_t));

        /* sum_future holds Σ_{j=k+1}^{n_prev-1} prev_row[j] for current k */
        seq_t sum_future = 0;

        /* compute new_row[k] for k = new_len-1 ... 1 */
        for (size_t k = new_len - 1; k >= 1; k--) {
            seq_t prev_kminus1 = prev_row[k - 1];
            seq_t prev_k = (k < n_prev) ? prev_row[k] : 0;
            new_row[k] = prev_kminus1 + 3 * prev_k + sum_future;
            if (k < n_prev) /* add prev_row[k] to sum_future for next iteration */
                sum_future += prev_k;
            if (k == 1) break; /* to avoid size_t underflow */
        }

        /* new_row[0] from T(n,0)-T(n,1)=2^n */
        new_row[0] = new_row[1] + pow2(current_row + 1);

        /* append new_row to flat vector */
        size_t old_flat_len = flat_len;
        flat_len += new_len;
        if (flat == NULL || flat_len > (size_t)-1) {
            /* should never happen with reasonable inputs */
            return 0;
        }
        flat = (seq_t*)realloc(flat, flat_len * sizeof(seq_t));
        for (size_t i = 0; i < new_len; ++i)
            flat[old_flat_len + i] = new_row[i];

        /* free old row, update pointers */
        free(prev_row);
        prev_row = new_row;
        prev_len = new_len;
        current_row++;
    }

    /* return requested term */
    return flat[n];
}
