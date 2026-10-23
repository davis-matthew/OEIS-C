/*
 * A046739  Triangle read by rows, related to number of permutations of [n]
 *          with 0 successions and k rises.
 * https://oeis.org/A046739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabf
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 84, u64 n <= 219,
 *            u128 n <= 577, big any n
 * Helpers:   global_to_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(n+1,r) = r*T(n,r) + (n+1-r)*T(n,r-1) + n*T(n-1,r-1)
   with base T(1,1)=0, T(2,1)=1.  The triangle read by rows gives the
   OEIS sequence A046739.  The function a(n) returns the nth term
   (offset 1). */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/global_to_coords.h"

/* static storage for triangle rows */
static seq_t **ROWS = NULL;        /* ROWS[row] -> array of T(row,*)   */
static size_t ROWS_CAP = 0;        /* capacity of ROWS array          */
static idx_t  MAX_ROW_COMPUTED = 0;/* largest row fully computed      */

/* ensure that rows 1..r are computed and stored */
static void ensure_rows(idx_t r)
{
    if (r <= MAX_ROW_COMPUTED)
        return;

    /* enlarge the ROWS array if necessary */
    size_t new_cap = ROWS_CAP;
    while (new_cap <= (size_t)r)
        new_cap = new_cap ? new_cap * 2 : 8;

    seq_t **tmp = (seq_t**)realloc(ROWS, new_cap * sizeof(seq_t*));
    if (!tmp)          /* allocation failed – leave unchanged */
        return;
    for (size_t i = ROWS_CAP; i < new_cap; ++i)
        tmp[i] = NULL;
    ROWS = tmp;
    ROWS_CAP = new_cap;

    /* initialise first two rows if needed */
    if (!ROWS[1]) {
        ROWS[1] = (seq_t*)malloc(sizeof(seq_t));
        if (!ROWS[1]) return;
        ROWS[1][0] = (seq_t)0;     /* T(1,1) = 0 */
    }
    if (!ROWS[2]) {
        ROWS[2] = (seq_t*)malloc(sizeof(seq_t));
        if (!ROWS[2]) return;
        ROWS[2][0] = (seq_t)1;     /* T(2,1) = 1 */
    }

    /* compute new rows */
    for (idx_t n = MAX_ROW_COMPUTED + 1; n <= r; ++n) {
        if (n <= 2)              /* rows 1 and 2 already set */
            continue;

        idx_t len = n - 1;       /* number of entries in row n */
        seq_t *arr = (seq_t*)malloc(len * sizeof(seq_t));
        if (!arr) return;        /* allocation failed */

        for (idx_t idx = 0; idx < len; ++idx) {
            idx_t col = idx + 1;     /* columns are 1‑based here */
            seq_t val = (seq_t)0;

            /* r * T(n-1,r) term – only when r <= (n-2) */
            if (idx < n - 2) {
                val += (seq_t)col * ROWS[n - 1][idx];
            }
            /* (n+1-r) * T(n-1,r-1) term – only when r > 1 */
            if (idx > 0 && idx - 1 < n - 2) {
                val += (seq_t)(n - col) * ROWS[n - 1][idx - 1];
            }
            /* n * T(n-2,r-1) term – only when r > 1 and n ≥ 3 */
            if (idx > 0 && idx - 1 < n - 3) {
                val += (seq_t)(n - 1) * ROWS[n - 2][idx - 1];
            }
            arr[idx] = val;
        }
        ROWS[n] = arr;
    }
    MAX_ROW_COMPUTED = r;
}

/* public API */
seq_t a(idx_t n)
{
    if (n == 0)            /* outside offset, undefined; return 0 */
        return (seq_t)0;

    idx_t row, col;
    global_to_coords(n, &row, &col);

    ensure_rows(row);

    return ROWS[row][col - 1];
}
