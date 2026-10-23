/*
 * A127156  Triangle read by rows: T(n,k) is the number of Dyck paths of
 *          semilength n starting with exactly k consecutive pyramids. A
 *          pyramid in a Dyck path is a factor of the form U^j D^j (j>0),
 *          starting at the x-axis. Here U=(1,1) and D=(1,-1). This definition
 *          differs from the one in A091866.
 * https://oeis.org/A127156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r(r+1)/2 ≤ n < (r+1)(r+2)/2, and T(n,k)
 *            satisfies the recurrence T(0,0)=1, T(n,k)=T(n-1,k)+T(n-1,k-1)
 *            for n,k≥1, T(n,0)=Catalan(n)-∑_{j=1}^{n}T(n,j)
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: the helper function catalan(idx_t) is provided in
   "../utils/catalan.h". */

#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n) {
    /* Static data to store already generated entries and the
       previous triangle row. */
    static seq_t *data = NULL;
    static size_t data_cap = 0;
    static size_t data_len = 0;

    static seq_t *prev_row = NULL;
    static idx_t prev_len = 0;      /* length of prev_row = row index + 1 */
    static idx_t next_row = 0;      /* index of next row to generate */

    /* Grow triangle until we have at least n+1 entries. */
    while (data_len <= n) {
        idx_t r = next_row;           /* current row index */
        idx_t cur_len = r + 1;        /* length of new row */

        /* Allocate array for the new row. */
        seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!cur) {
            /* Out of memory - abort. */
            abort();
        }

        /* Compute entries k ≥ 1 using the recurrence. */
        seq_t sum_rest = 0;
        for (idx_t k = 1; k <= r; ++k) {
            seq_t left  = (k   < prev_len) ? prev_row[(size_t)k]   : 0;
            seq_t right = (k-1 < prev_len) ? prev_row[(size_t)(k-1)] : 0;
            cur[(size_t)k] = left + right;
            sum_rest += cur[(size_t)k];
        }

        /* Determine T(r,0).  The sum of the row is a Catalan number. */
        cur[0] = catalan(r) - sum_rest;

        /* Ensure that the data array can hold the new entries. */
        if (data_len + cur_len > data_cap) {
            size_t new_cap = data_cap ? data_cap * 2 : 16;
            while (new_cap < data_len + cur_len) new_cap *= 2;
            seq_t *new_data = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
            if (!new_data) abort();
            data = new_data;
            data_cap = new_cap;
        }

        /* Copy the new row into the flat data array. */
        for (idx_t i = 0; i < cur_len; ++i) {
            data[data_len + (size_t)i] = cur[(size_t)i];
        }
        data_len += cur_len;

        /* Prepare for the next row.  The previous row is no longer needed. */
        free(prev_row);
        prev_row = cur;
        prev_len = cur_len;
        ++next_row;
    }

    return data[(size_t)n];
}
