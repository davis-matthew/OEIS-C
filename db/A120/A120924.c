/*
 * A120924  Triangle read by rows: T(n,k) is the number of ternary words of
 *          length n on {0,1,2}, having k isolated 0's (n >= 0, k >= 0).
 * https://oeis.org/A120924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Triangle read by rows: number of ternary words of length n
 *            having k isolated 0's. The function returns a(n) where the AOIS
 *            sequence a(n) is the flattened triangle with row r starting at
 *            index offset_row(r)
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --- static state ---------------------------------------------------- */

static idx_t current_row = 0;          /* last row that has been fully computed   */
static idx_t offset_start = 0;         /* linear index of the first element of current_row   */
static idx_t cum_end = 1;              /* linear index of the element after current_row    */

static idx_t dp_cap = 0;               /* current capacity of DP arrays                */

/* DP arrays for two consecutive rows, split by state 0,1,2   */
static seq_t *dpPrev0 = NULL, *dpPrev1 = NULL, *dpPrev2 = NULL;
static seq_t *dpCurr0 = NULL, *dpCurr1 = NULL, *dpCurr2 = NULL;

/* ------------------------------------------------------------------- */
/* Ensure DP arrays are large enough to hold indices up to 'need'      */
static void ensure_dp_capacity(idx_t need) {
    if (dp_cap < need) {
        idx_t new_cap = dp_cap ? dp_cap << 1 : 16;
        while (new_cap < need)
            new_cap <<= 1;
        dpPrev0 = (seq_t*)realloc(dpPrev0, new_cap * sizeof(seq_t));
        dpPrev1 = (seq_t*)realloc(dpPrev1, new_cap * sizeof(seq_t));
        dpPrev2 = (seq_t*)realloc(dpPrev2, new_cap * sizeof(seq_t));
        dpCurr0 = (seq_t*)realloc(dpCurr0, new_cap * sizeof(seq_t));
        dpCurr1 = (seq_t*)realloc(dpCurr1, new_cap * sizeof(seq_t));
        dpCurr2 = (seq_t*)realloc(dpCurr2, new_cap * sizeof(seq_t));
        /* Zero the newly allocated part */
        for (idx_t i = dp_cap; i < new_cap; ++i) {
            dpPrev0[i] = 0;  dpPrev1[i] = 0;  dpPrev2[i] = 0;
            dpCurr0[i] = 0;  dpCurr1[i] = 0;  dpCurr2[i] = 0;
        }
        dp_cap = new_cap;
    }
}

/* ------------------------------------------------------------------- */
/* Precompute rows up to and including the row that contains target   */
static void ensure_precomputed(idx_t target_index) {
    /* First time: initialise row 0 */
    if (current_row == 0 && dpPrev0 == NULL) {
        ensure_dp_capacity(1);
        dpPrev0[0] = (seq_t)1;   /* word of length 0, no isolated 0 */
        dpPrev1[0] = 0;
        dpPrev2[0] = 0;
    }

    /* Compute rows until the target index lies within the current row */
    while (cum_end <= target_index) {
        idx_t next_row = current_row + 1;
        idx_t kmax_prev = (current_row + 1) / 2;            /* ceil(current_row/2) */
        idx_t kmax_next = (next_row + 1) / 2;              /* ceil(next_row/2) */
        idx_t row_terms = kmax_next + 1;

        /* Make sure DP arrays have enough space */
        ensure_dp_capacity(kmax_next + 1);

        /* Clear current DP arrays for indices [0, kmax_next] */
        for (idx_t i = 0; i <= kmax_next; ++i) {
            dpCurr0[i] = 0;
            dpCurr1[i] = 0;
            dpCurr2[i] = 0;
        }

        /* Transitions from dpPrev to dpCurr */
        for (idx_t k = 0; k <= kmax_prev; ++k) {
            seq_t v0 = dpPrev0[k];
            seq_t v1 = dpPrev1[k];
            seq_t v2 = dpPrev2[k];

            if (v0) {
                /* add 0 -> state 1, k+1 */
                if (k + 1 <= kmax_next)
                    dpCurr1[k + 1] += v0;
                /* add 1 or 2 -> state 0, k */
                dpCurr0[k] += v0 * (seq_t)2;
            }
            if (v1) {
                /* add 0 -> state 2, k-1   (k>=1 guaranteed) */
                if (k >= 1)
                    dpCurr2[k - 1] += v1;
                /* add 1 or 2 -> state 0, k */
                dpCurr0[k] += v1 * (seq_t)2;
            }
            if (v2) {
                /* add 0 -> state 2, k */
                dpCurr2[k] += v2;
                /* add 1 or 2 -> state 0, k */
                dpCurr0[k] += v2 * (seq_t)2;
            }
        }

        /* Update indices for next row */
        idx_t next_row_start = cum_end;     /* start of the newly computed row */
        cum_end += row_terms;               /* end index of the new row (exclusive) */
        offset_start = next_row_start;      /* start of the new current row */

        /* Make dpCurr the new dpPrev for the next iteration */
        seq_t *tmp = dpPrev0; dpPrev0 = dpCurr0; dpCurr0 = tmp;
        tmp = dpPrev1; dpPrev1 = dpCurr1; dpCurr1 = tmp;
        tmp = dpPrev2; dpPrev2 = dpCurr2; dpCurr2 = tmp;

        current_row = next_row;
    }
}

/* ------------------------------------------------------------------- */
/* Public routine ----------------------------------------------------- */

seq_t a(idx_t n) {
    ensure_precomputed(n);
    idx_t k = n - offset_start;                /* position within the current row */
    return dpPrev0[k] + dpPrev1[k] + dpPrev2[k];
}
