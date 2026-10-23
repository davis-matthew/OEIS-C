/*
 * A131254  A004070 * A000012.
 * https://oeis.org/A131254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the flattened triangle A004070*A000012.
 *            The triangle rows T(n,k) satisfy T(0,0)=1; T(n,0)=T(n,n)=1 for
 *            n>0; T(n,k)=T(n-1,k-1)+T(n-2,k-1) for 1≤k≤n-1. The flattened
 *            sequence a(n) is obtained by: for each row n, let D(n,k)=T(n,k)
 *            for 0≤k≤n-1 and D(n,n)=1; compute the row val...
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Caches for the flattened sequence and the last two triangle rows */
    static seq_t  *flatten     = NULL;
    static size_t flatten_len  = 0;
    static size_t flatten_cap  = 0;
    static seq_t  *prev_prev   = NULL;
    static size_t len_prev_prev= 0;
    static seq_t  *prev       = NULL;
    static size_t len_prev    = 0;
    static size_t next_row    = 0;   /* index of the next row to generate */

    /* Ensure we have computed at least (n+1) elements in the flattened array */
    while (flatten_len <= (size_t)n) {
        /* Allocate current row of triangle data */
        size_t cur_len = next_row + 1;
        seq_t *cur = (seq_t*)malloc(cur_len * sizeof(seq_t));
        if (!cur) { /* allocation failure – abort */
            return (seq_t)0;
        }

        /* Build the triangle row */
        if (next_row == 0) {                 /* Row 0: 1 */
            cur[0] = (seq_t)1;
        } else if (next_row == 1) {          /* Row 1: 1,1 */
            cur[0] = (seq_t)1;
            cur[1] = (seq_t)1;
        } else {                              /* Row n (n≥2) */
            cur[0] = (seq_t)1;
            cur[cur_len-1] = (seq_t)1;
            for (size_t k = 1; k < cur_len-1; ++k) {
                cur[k] = prev[k-1] + prev_prev[k-1];
            }
        }

        /* Compute the flattened values of this row (cumulative sums from right) */
        seq_t *row_final = (seq_t*)malloc(cur_len * sizeof(seq_t));
        row_final[cur_len-1] = (seq_t)1;                    /* last element is always 1 */
        for (int j = (int)cur_len - 2; j >= 0; --j) {
            row_final[j] = row_final[j+1] + cur[j];
        }

        /* Extend the flattened array if necessary */
        size_t needed = flatten_len + cur_len;
        if (needed > flatten_cap) {
            size_t new_cap = flatten_cap? flatten_cap*2 : 128;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(flatten, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;            /* allocation failure */
            flatten = tmp;
            flatten_cap = new_cap;
        }

        /* Append the new row to the flattened array */
        for (size_t j = 0; j < cur_len; ++j) {
            flatten[flatten_len++] = row_final[j];
        }

        /* Free temporary structures */
        free(row_final);

        /* Update cached rows for next iteration */
        if (next_row == 0) {
            /* row_prev_prev becomes row 0 */
            prev_prev = cur;
            len_prev_prev = cur_len;
        } else if (next_row == 1) {
            /* row_prev becomes row 1; keep prev_prev as row 0 */
            prev = cur;
            len_prev = cur_len;
        } else {
            /* For n≥2, drop the oldest row (prev_prev) */
            seq_t *old_prev_prev = prev_prev;
            seq_t *old_prev      = prev;
            free(old_prev_prev);
            prev_prev = old_prev;
            len_prev_prev = len_prev;
            prev = cur;
            len_prev = cur_len;
        }

        /* Prepare for generating the next row */
        ++next_row;
    }

    return flatten[(size_t)n];
}
