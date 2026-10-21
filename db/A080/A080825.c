/*
 * A080825  Triangle read by rows in which n-th row gives trajectory of n
 *          (omitting n itself) under the map k -> k-1 if k odd, k -> k/2 if k
 *          even.
 * https://oeis.org/A080825
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   traj_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th element of the triangle formed by the
 * trajectories of integers under the map k → k‑1 if k is odd, k → k/2
 * if k is even.  Each row n (n ≥ 2) contains the trajectory of n,
 * omitting the starting value itself.  The triangle is read row by
 * row, forming the sequence a(2), a(3), a(4), …  Offset is 2. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/traj_len.h"

/* The build system supplies the actual definitions of seq_t and idx_t.
 * They are not redefined here. */

/* Dynamic storage for cumulative counts of terms after each processed row.
 * cum[i] = total number of terms up to and including row (i+2). */
static idx_t *cum = NULL;      /* cumulative counts array   */
static size_t cum_cap = 0;     /* allocated capacity        */
static size_t cum_used = 0;    /* number of rows stored     */
static idx_t last_row = 1;     /* last row processed (starts before 2) */

/* Ensure cum[] covers at least the position pos in the flattened triangle. */
static void ensure_cumulative(idx_t pos)
{
    /* Allocate the array if this is the first call. */
    if (!cum) {
        cum_cap = 64;
        cum = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        if (!cum) return;                  /* allocation failure – leave cum NULL */
    }

    while (cum_used == 0 || cum[cum_used - 1] <= pos) {
        /* Process the next row. */
        idx_t m   = last_row + 1;          /* original number of the new row     */
        idx_t len = traj_len(m);           /* length of its trajectory           */
        idx_t new_total = (cum_used == 0) ? len : cum[cum_used - 1] + len;

        /* Grow the array if necessary. */
        if (cum_used >= cum_cap) {
            size_t new_cap = cum_cap * 2;
            idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            if (!tmp) {                    /* allocation failure – free & reset */
                free(cum);
                cum = NULL;
                cum_cap = 0;
                cum_used = 0;
                return;
            }
            cum = tmp;
            cum_cap = new_cap;
        }

        cum[cum_used++] = new_total;
        last_row = m;
    }
}

/* External function defined by the build system. */
seq_t a(idx_t n)
{
    if (n < 2) {
        /* Undefined below the offset; return 0. */
        return (seq_t)0;
    }

    /* Zero‑based position within the flattened triangle. */
    idx_t pos = n - 2;

    /* Make sure cum[] contains a row for this position. */
    ensure_cumulative(pos);
    if (!cum) return (seq_t)0;          /* allocation failure fallback */

    /* Find the row that contains pos. */
    size_t row_index = 0;
    while (row_index < cum_used && pos >= cum[row_index]) {
        ++row_index;
    }

    /* row_index now points to the first entry with cum[row_index] > pos. */
    idx_t m = (idx_t)row_index + 2;                          /* original number for this row */
    idx_t prev_cum = (row_index == 0) ? 0 : cum[row_index - 1];
    idx_t offset = pos - prev_cum;                           /* zero‑based offset within the row */

    /* Compute the value at this offset: apply offset+1 steps to m. */
    idx_t cur = m;
    for (idx_t i = 0; i <= offset; ++i) {
        if (cur & 1U)
            cur -= 1U;
        else
            cur >>= 1U;
    }

    return (seq_t)cur;
}
