/*
 * A121692  Triangle read by rows: T(n,k) is the number of deco polyominoes of
 *          height n and vertical height (i.e., number of rows) k (1 <= k <=
 *          n). A deco polyomino is a directed column-convex polyomino in
 *          which the height, measured along the diagonal, is attained only in
 *          the last column.
 * https://oeis.org/A121692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 97, u64 n <= 242,
 *            u128 n <= 617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A121692 – Triangle of deco polyominoes.
 *
 *  The recurrence (from OEIS) for the triangle entries is:
 *      T(n,1) = 1              for n ≥ 1
 *      T(n,n) = 1              for n ≥ 1
 *      T(n,k) = k·T(n-1,k) + 2·T(n-1,k-1) + Σ_{j=1}^{k-2} T(n-1,j)
 *          for 2 ≤ k ≤ n−1.
 *
 *  The sequence a(n) is the triangle read by rows (row 1 first, then row 2,
 *  etc.).  Its first several values are:
 *      1, 1, 1, 1, 4, 1, 1, 10, 12, 1, 1, 22, 57, 39, 1, …
 *
 *  The function below caches the triangle entries as they are needed,
 *  so any order of calls is supported.  It works with the generic
 *  `seq_t` type supplied by the build system, and with `idx_t = uint64_t`.
 */
#include <stdlib.h>          /* size_t, malloc, realloc, free */

static inline idx_t triangular(idx_t r)
{
    return (r * (r + 1)) / 2;
}

/* Cache of all triangle entries up to the largest row that has
 * been computed.  Entries are stored consecutively:
 *   row 1: index 0 … 0
 *   row 2: index 1 … 2
 *   row 3: index 3 … 5
 *   ...
 */
static seq_t  *triangle_table = NULL;   /* pointer to the array */
static idx_t   triangle_rows   = 0;     /* last fully computed row (≥0) */
static size_t  triangle_cap    = 0;     /* array capacity in elements */

/* Ensure that rows 1 … target_row have been computed. */
static void grow_triangle(idx_t target_row)
{
    if (target_row <= triangle_rows)          /* already ready */
        return;

    /* New capacity needed: number of elements in rows 1 … target_row */
    size_t new_cap = (size_t)triangular(target_row);
    seq_t *tmp;

    if (triangle_table == NULL) {
        /* First allocation */
        tmp = (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);  /* allocation failed */
    } else {
        /* Reallocate to the larger size */
        tmp = (seq_t *)realloc(triangle_table, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);  /* allocation failed */
    }
    triangle_table = tmp;
    triangle_cap   = new_cap;

    /* Compute the missing rows. */
    for (idx_t r = triangle_rows + 1; r <= target_row; ++r) {
        /* Index of first element of row r in the table */
        idx_t cur_start  = triangular(r - 1);
        idx_t prev_start = (r >= 2) ? triangular(r - 2) : 0U;

        /* k = 1 : always 1 */
        triangle_table[cur_start] = (seq_t)1;

        /* For k = 2 … r-1 */
        seq_t sum_before = 0;          /* Σ_{j=1}^{k-2} T(r-1,j) */
        if (r >= 3) {
            for (idx_t k = 2; k <= r - 1; ++k) {
                seq_t t_k   = triangle_table[prev_start + (k - 1)];
                seq_t t_km1 = triangle_table[prev_start + (k - 2)];
                seq_t val   = (seq_t)k * t_k + (seq_t)2 * t_km1 + sum_before;
                triangle_table[cur_start + (k - 1)] = val;
                sum_before += t_km1;                /* prepare for next k */
            }
        }
        /* k = r : always 1 */
        triangle_table[cur_start + (r - 1)] = (seq_t)1;
    }

    triangle_rows = target_row;
}

/* Return the nth term of the flattened triangle (a(1) = T(1,1)). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n = 0, safety */

    /* Find the row that contains the nth entry:
     *   triangular(r-1) < n ≤ triangular(r)
     */
    idx_t r      = 1;
    idx_t tri_r  = 1;                 /* triangular(1) */
    while (tri_r < n) {
        ++r;
        tri_r += r;                   /* now tri_r = triangular(r) */
    }

    /* Compute needed rows */
    grow_triangle(r);

    /* Offset within the row (1‑based) */
    idx_t offset = n - triangular(r - 1);

    /* Return the stored value */
    return triangle_table[triangular(r - 1) + (offset - 1)];
}
