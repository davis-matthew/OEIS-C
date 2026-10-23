/*
 * A193091  Augmentation of the triangular array A158405. See Comments.
 * https://oeis.org/A193091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A193091 – Augmentation of the triangular array A158405.
    The underlying triangular array T is defined by:
        T(0,0) = 1
        T(n,0) = 1  for n > 0
        T(n,k) = Σ_{i=0}^{min(k,n-1)} (2*k-2*i+1) * T(n-1,i)   for 0 < k ≤ n
    The sequence a(n) is the flattening of this triangle row‑by‑row
    (row 0 first, then row 1, …).  Example rows:
        1
        1 3
        1 6 14
        1 9 37 79
        1 12 69 242 494
    The implementation uses a static cache of all computed rows so that
    multiple calls in any order run in overall O(r²) time, where
    r ≈ √(2n).  No external libraries are required apart from <stdlib.h>. */

#include <stdlib.h>

/* Static cache of the triangular array.
   tri[i] stores the i‑th entry of the flattened triangle. */
static seq_t *tri = NULL;
static idx_t  computed_rows = 0;   /* number of rows already computed (next to fill) */
static size_t tri_cap      = 0;    /* current capacity of tri (in seq_t elements) */

/* Return the linear index where the row r starts.  row 0 starts at 0. */
static inline size_t row_start(idx_t r)
{
    return (size_t)((r * (r + 1)) / 2);
}

/* Ensure the cache has room for at least ‘needed’ seq_t elements. */
static void ensure_capacity(size_t needed)
{
    if (tri_cap >= needed)
        return;
    size_t new_cap = tri_cap ? tri_cap * 2 : 1024;
    if (new_cap < needed)
        new_cap = needed;
    tri = (seq_t *)realloc(tri, new_cap * sizeof(seq_t));
    tri_cap = new_cap;
}

/* Compute rows 0 … r (inclusive) if they have not already been computed. */
static void ensure_rows(idx_t r)
{
    if (computed_rows > r)
        return;

    size_t needed = (size_t)(((r + 1) * (r + 2)) / 2);
    ensure_capacity(needed);

    for (idx_t row = computed_rows; row <= r; ++row) {
        size_t start = row_start(row);
        if (row == 0) {                /* base row */
            tri[start] = 1;
        } else {
            size_t prev_start = row_start(row - 1);
            tri[start] = 1;            /* T(row,0) = 1 for all rows > 0 */
            for (idx_t k = 1; k <= row; ++k) {
                seq_t sum = 0;
                /* T(row-1,i) exists only for i <= row-1 */
                idx_t max_i = (k <= row - 1) ? k : (row - 1);
                for (idx_t i = 0; i <= max_i; ++i) {
                    seq_t coeff = (seq_t)(2 * k - 2 * i + 1);
                    sum += coeff * tri[prev_start + i];
                }
                tri[start + k] = sum;
            }
        }
    }
    computed_rows = r + 1;
}

/* Public function: return a(n) */
seq_t a(idx_t n)
{
    /* Locate the row r containing position n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    size_t start = row_start(r);
    idx_t   k     = n - (idx_t)start;   /* column within the row */

    ensure_rows(r);
    return tri[start + k];
}
