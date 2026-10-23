/*
 * A101842  Triangle read by rows: T(n,k), for k=-n..n-1, is the scaled (by
 *          2^n n!) probability that the sum of n uniform [-1, 1] variables is
 *          between k and k+1.
 * https://oeis.org/A101842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) forms the triangle read by rows T(n,k) where
 *            T(1,-1)=T(1,0)=1 and for n>1 T(n,k) = (n-k)*T(n-1,k-1) +
 *            T(n-1,k) + (n+k+1)*T(n-1,k+1), k = -n .. n-1. The sequence a(n)
 *            is the elements of this triangle written row by row
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 118, u64 n <= 318,
 *            u128 n <= 838, big any n
 * Helpers:   needed_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/needed_row.h"

static seq_t *prev_row = NULL;   /* current row (1‑based row number r_cur)   */
static idx_t  r_cur     = 0;     /* current row number, 0 means uninitialised */
static idx_t  prev_len  = 0;     /* length of current row = 2 * r_cur */

/* build rows up to target (inclusive) */
static void grow_to(idx_t target)
{
    /* initialise first row if needed */
    if (r_cur == 0) {
        prev_len = 2;
        prev_row = (seq_t*)malloc(prev_len * sizeof(seq_t));
        prev_row[0] = (seq_t)1;
        prev_row[1] = (seq_t)1;
        r_cur = 1;
    }

    while (r_cur < target) {
        int next_r   = (int)(r_cur) + 1;       /* new row number (fits in int) */
        idx_t new_len = prev_len + 2;          /* 2 * next_r */
        seq_t *new_row = (seq_t*)malloc(new_len * sizeof(seq_t));

        for (idx_t j = 0; j < new_len; ++j) {
            int k = -next_r + (int)j;          /* column index for the new row */
            seq_t val = 0;

            /* T(next_r-1, k-1) term */
            if (k - 1 >= -(next_r - 1) && k - 1 <= next_r - 2) {
                idx_t idx1 = (idx_t)(k - 1 + (next_r - 1));
                val += (seq_t)(next_r - k) * prev_row[idx1];
            }
            /* T(next_r-1, k) term */
            if (k >= -(next_r - 1) && k <= next_r - 2) {
                idx_t idx2 = (idx_t)(k + (next_r - 1));
                val += prev_row[idx2];
            }
            /* T(next_r-1, k+1) term */
            if (k + 1 >= -(next_r - 1) && k + 1 <= next_r - 2) {
                idx_t idx3 = (idx_t)(k + 1 + (next_r - 1));
                val += (seq_t)(next_r + k + 1) * prev_row[idx3];
            }
            new_row[j] = val;
        }

        free(prev_row);
        prev_row = new_row;
        r_cur    = next_r;
        prev_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* defined only for n ≥ 1, but safe for 0 */
        return (seq_t)0;

    idx_t target = needed_row(n);
    grow_to(target);

    /* position within the target row (1‑based) */
    idx_t pos = n - target * (target - 1);
    return prev_row[pos - 1];
}
