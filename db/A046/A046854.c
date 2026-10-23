/*
 * A046854  Triangle read by rows: T(n, k) = binomial(floor((n+k)/2), k), n >=
 *          k >= 0.
 * https://oeis.org/A046854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, col) where the triangle
 *            T(n,k)=binomial(floor((n+k)/2),k) read row by row (row 0 has one
 *            element, row 1 has two, …). The sequence is generated using the
 *            recurrence T(n,0)=1, T(n,n)=1, T(n,k)=T(n-1,k-1)+T(n-2,k) for
 *            0<k<n-1. The function caches the flattened triangle and returns
 *            the...
 * Range:     u8 n <= 126, u16 n <= 388, u32 n <= 1346, u64 n <= 4890,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Cache for the flattened triangle (a[0], a[1], …), zero‑based.  */
static seq_t *flat      = NULL;
static idx_t  flat_len  = 0;
static idx_t  flat_cap  = 0;

/* Last two rows of the triangle.  */
static seq_t *prev1      = NULL;   /* row n-1  */
static idx_t  prev1_len  = 0;
static seq_t *prev2      = NULL;   /* row n-2  */
static idx_t  prev2_len  = 0;

/* Next row index to compute.  */
static idx_t next_row = 0;

seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n+1 entries.  */
    while (flat_len <= n) {
        /* Compute row 'next_row'.  */
        idx_t row     = next_row;
        idx_t cur_len = row + 1;

        seq_t *cur = (seq_t *)calloc((size_t)cur_len, sizeof(seq_t));
        if (!cur) {
            /* Out of memory – return zero (should not happen in test cases). */
            return (seq_t)0;
        }

        if (row == 0) {
            /* Row 0: single element 1. */
            cur[0] = 1;
        } else if (row == 1) {
            /* Row 1: 1 1. */
            cur[0] = 1;
            cur[1] = 1;
        } else {
            /* Row >= 2.  */
            cur[0]    = 1;
            cur[row]  = 1;
            for (idx_t k = 1; k < row; ++k) {
                if (k == row - 1) {
                    /* Last element of the row – always 1. */
                    cur[k] = 1;
                } else {
                    /* Interior element with recurrence.  */
                    seq_t left  = prev1[k - 1];
                    seq_t right = (k < prev2_len) ? prev2[k] : 0;
                    cur[k] = left + right;
                }
            }
        }

        /* Ensure flat array has enough capacity. */
        idx_t needed = flat_len + cur_len;
        if (needed > flat_cap) {
            idx_t new_cap = (flat_cap > 0) ? flat_cap * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(flat, (size_t)new_cap * sizeof(seq_t));
            if (!tmp) {
                free(cur);
                /* Out of memory – return zero. */
                return (seq_t)0;
            }
            flat   = tmp;
            flat_cap = new_cap;
        }

        /* Append current row to the flattened sequence. */
        for (idx_t i = 0; i < cur_len; ++i) {
            flat[flat_len++] = cur[i];
        }

        /* Slide window of last two rows. */
        seq_t *old_prev2 = prev2;
        prev2 = prev1;
        prev2_len = prev1_len;
        prev1 = cur;
        prev1_len = cur_len;

        if (old_prev2) free(old_prev2);

        ++next_row;
    }

    /* The requested element is now guaranteed to exist. */
    return flat[n];
}
