/*
 * A059283  Triangle T(n,k) (0<= k <=n) read by rows. Left edge is 1, 0, 0,
 *          ... Otherwise each entry is sum of entry to left, entries
 *          immediately above it to left and right and entry directly above it
 *          2 rows back.
 * https://oeis.org/A059283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 133, u64 n <= 430,
 *            u128 n <= 1427, big any n
 * Helpers:   compute_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* *Sequence A059283 – Triangle T(n,k)*  
 *
 *   T(0,0) = 1
 *   T(n,0) = 0     for n > 0
 *   T(n,k) = T(n,k‑1) + T(n‑1,k‑1) + T(n‑1,k) + T(n‑2,k‑1)
 *             (n > 0, k > 0)
 *
 *   a(n) is the flattened triangle row‑major,
 *   i.e. a(n) = T(row(n), col(n))
 *   where row(n) = largest r with r(r+1)/2 ≤ n
 *   and col(n) = n – r(r+1)/2.
 *
 * The implementation lazily builds the triangle, memoising all
 * computed rows so that any a(n) can be retrieved in O(1) after the
 * required rows are built (overall time O(r²) where r ≈ √(2n)).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/compute_row.h"

/* ------------------------------------------------------------------ */
/* Internal storage – dynamically grown arrays                       */
/* ------------------------------------------------------------------ */
static bool  initialized = false;      /* one‑time init flag        */
static seq_t *tri = NULL;              /* flattened triangle array  */
static idx_t tri_len = 0, tri_cap = 0; /* count / capacity of tri    */
static idx_t *row_start = NULL;        /* start index of each row   */
static idx_t start_cap = 0, rows_done = 0;

/* ------------------------------------------------------------------ */
/* Helper routine to initialise the global arrays once                */
/* ------------------------------------------------------------------ */
static void init_once(void)
{
    if (initialized)
        return;
    initialized = true;

    /* initial capacity for row_start – will grow as needed      */
    start_cap = 32;
    row_start = (idx_t *)malloc((size_t)start_cap * sizeof(idx_t));
    if (!row_start)
        exit(EXIT_FAILURE);
    row_start[0] = 0;   /* row 0 starts at index 0 */

    /* initial capacity for tri – will grow as needed            */
    tri_cap = 16;
    tri = (seq_t *)malloc((size_t)tri_cap * sizeof(seq_t));
    if (!tri)
        exit(EXIT_FAILURE);
    tri[0] = (seq_t)1; /* T(0,0) = 1 */
    tri_len  = 1;
    rows_done = 0;     /* only row 0 known                                      */
}

/* ------------------------------------------------------------------ */
/* Compute integer row number for a flat index n                       */

/* ------------------------------------------------------------------ */
/* Ensure that rows 0..r are fully computed in the triangle array   */
/* ------------------------------------------------------------------ */
static void ensure_rows(idx_t r)
{
    init_once();

    while (rows_done < r) {
        idx_t   next = rows_done + 1;
        idx_t   start_idx = tri_len;      /* first index of new row   */

        /* grow row_start capacity if needed */
        if (start_cap <= next) {
            size_t new_cap = start_cap ? (size_t)start_cap * 2 : 32;
            while (new_cap <= (size_t)next)
                new_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(row_start, new_cap * sizeof(idx_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            row_start = tmp;
            start_cap = (idx_t)new_cap;
        }
        row_start[next] = start_idx;

        /* grow tri capacity to hold the new row (length next+1)      */
        size_t need = tri_len + (size_t)(next + 1);
        if (tri_cap < need) {
            size_t new_cap = tri_cap ? (size_t)tri_cap * 2 : 16;
            while (new_cap < need)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(tri, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            tri = tmp;
            tri_cap = (idx_t)new_cap;
        }

        /* k = 0 entry – left edge of the new row */
        tri[tri_len++] = (seq_t)0;

        /* compute remaining entries k = 1 .. next */
        for (idx_t k = 1; k <= next; ++k) {
            seq_t sum = tri[start_idx + (k - 1)];                       /* T(r, k‑1)      */
            sum += tri[row_start[next - 1] + (k - 1)];                   /* T(r‑1, k‑1)    */
            if (k <= next - 1)
                sum += tri[row_start[next - 1] + k];                    /* T(r‑1, k)      */
            if (next >= 2 && (k - 1) <= next - 2)
                sum += tri[row_start[next - 2] + (k - 1)];              /* T(r‑2, k‑1)    */
            tri[tri_len++] = sum;
        }

        rows_done = next;
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point – returns the a(n) term of the sequence         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t r = compute_row(n);
    ensure_rows(r);
    /* tri[n] is now guaranteed to be valid */
    return tri[(size_t)n];
}
