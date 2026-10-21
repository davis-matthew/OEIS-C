/*
 * A222403  Triangle read by rows: left and right edges are A000217, interior
 *          entries are filled in using the Pascal triangle rule.
 * https://oeis.org/A222403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element a row‑by‑row read triangle whose left and
 *            right edges are the triangular numbers T(r)=r(r+1)/2 and whose
 *            interior entries satisfy the Pascal rule
 * Range:     u8 n <= 45, u16 n <= 159, u32 n <= 608, u64 n <= 2240,
 *            u128 n <= 8577, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t **triangle = NULL;   /* pointers to stored rows   */
static size_t triangle_rows = 0;   /* number of rows currently in use */
static size_t triangle_cap  = 0;   /* allocated capacity of the pointer array */

/* Helper: allocate an array of seq_t */
static seq_t *alloc_seq_array(size_t n)
{
#if defined(__cplusplus)
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Helper: reallocate the pointer array holding the rows */
static void grow_triangle_ptrs(size_t needed_cap)
{
    if (triangle_cap >= needed_cap)
        return;
    size_t new_cap = triangle_cap > 0 ? triangle_cap * 2 : 4;
    if (new_cap < needed_cap)
        new_cap = needed_cap;
#if defined(__cplusplus)
    seq_t **tmp = new seq_t*[new_cap];
    for (size_t i = 0; i < triangle_rows; ++i)
        tmp[i] = triangle[i];
    delete[] triangle;
    triangle = tmp;
#else
    triangle = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
#endif
    triangle_cap = new_cap;
}

/* Find the row index r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = n;      /* hi can safely be n for 64‑bit n   */
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri <= n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* Ensure that all rows up to and including r have been computed */
static void ensure_rows_up_to(idx_t r)
{
    while (triangle_rows <= (size_t)r) {
        size_t idx = triangle_rows;         /* current row index   */
        size_t len = idx + 1;               /* number of elements in the row */
        seq_t *row = alloc_seq_array(len);

        seq_t tri = (seq_t)idx * ((seq_t)idx + 1) / 2;
        row[0] = tri;
        if (len > 1) {
            seq_t *prev = triangle[idx - 1];
            for (size_t k = 1; k < idx; ++k) {
                row[k] = prev[k - 1] + prev[k];
            }
            row[idx] = tri;
        }

        grow_triangle_ptrs(triangle_rows + 1);
        triangle[triangle_rows++] = row;
    }
}

/* The requested sequence value */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t k = n - (idx_t)r * ((idx_t)r + 1) / 2;
    ensure_rows_up_to(r);
    return triangle[r][k];
}
