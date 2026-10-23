/*
 * A166973  Triangle T(n,k) read by rows: T(n, k) = (m*n - m*k + 1)*T(n - 1, k
 *          - 1) + (5*k - 4)*(m*k - (m - 1))*T(n - 1, k) where m = 0.
 * https://oeis.org/A166973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the number of non‑nonoagnetic sub‑permutations, defined
 *            by the recurrence T(n, k) = T(n-1, k-1) + (5*k-4)*T(n-1, k) with
 *            T(1,1)=1. a(n) is the concatenation of the rows of this
 *            triangle; i.e. a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2),
 *            a(4)=T(3,1),
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 68, u64 n <= 175,
 *            u128 n <= 325, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*, size_t, size_t) */

static idx_t max_row = 0;          /* highest row already computed */
static seq_t *tri = NULL;          /* packed triangle values */
static size_t tri_cap = 0;         /* allocated number of cells */

/* Ensure the triangle contains all rows up to target_row (inclusive). */
static void build_to(idx_t target_row)
{
    if (target_row <= max_row)
        return;

    /* How many cells are needed up to target_row? */
    size_t new_cap = (size_t)target_row * (size_t)(target_row + 1) / 2;

    /* Reallocate storage, preserving existing data. */
    seq_t *new_tri = seq_realloc(tri, tri_cap, new_cap);
    if (!new_tri)                     /* silently fail if out of memory */
        return;
    tri = new_tri;
    tri_cap = new_cap;

    for (idx_t r = max_row + 1; r <= target_row; ++r) {
        idx_t curr_start = r * (r - 1) / 2;
        idx_t prev_start = (r - 1) * (r - 2) / 2;

        if (r == 1) {
            tri[0] = (seq_t)1;
        } else {
            for (idx_t c = 1; c <= r; ++c) {
                seq_t val = 0;
                if (c > 1)                                 /* T(r-1,c-1) */
                    val += tri[prev_start + (c - 2)];
                if (c <= r - 1) {                          /* (5c-4)*T(r-1,c) */
                    seq_t coeff = (seq_t)(5 * c - 4);
                    val += coeff * tri[prev_start + (c - 1)];
                }
                tri[curr_start + (c - 1)] = val;
            }
        }
    }
    max_row = target_row;
}

/* Return the n-th term of A166973 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n)
        ++r;

    build_to(r);
    if (r == 0)          /* allocation failed */
        return (seq_t)0;

    idx_t prev_start = r * (r - 1) / 2;
    idx_t col = n - prev_start;            /* 1‑based column within the row */
    idx_t index = prev_start + (col - 1);

    return tri[(size_t)index];
}
