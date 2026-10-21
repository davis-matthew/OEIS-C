/*
 * A208756  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208755; see the Formula section.
 * https://oeis.org/A208756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   row_start
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle coefficients of v(n,x) (OEIS A208756)
   Recurrence:
     T(1,0) = 1,
     T(2,0) = 0, T(2,1) = 2,
     for n ≥ 3 and 0 ≤ k ≤ n-1
       T(n,k) = T(n-1,k) + T(n-1,k-1) - T(n-2,k-1) + 2·T(n-2,k-2)
   The sequence a(n) is the entire triangle written row‑by‑row
   (a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1), a(4)=T(3,0), …).  */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/row_start.h"

/* cached triangle */
static seq_t *tri_cache = NULL;   /* flattened triangle storage */
static size_t   cache_cap  = 0;   /* allocated size in elements */
static size_t   rows_done  = 0;   /* rows that have already been computed */

/* Ensure rows 1 … target are present in tri_cache */
static void ensure_rows(size_t target)
{
    if (target <= rows_done)
        return;

    /* total number of entries needed up to target row */
    size_t required = target * (target + 1) / 2;
    if (required > cache_cap) {
        cache_cap = required;
        tri_cache = (seq_t *)realloc(tri_cache, cache_cap * sizeof(seq_t));
    }

    /* compute new rows */
    for (size_t r = rows_done + 1; r <= target; ++r) {
        size_t start = row_start(r);

        if (r == 1) {
            tri_cache[start] = (seq_t)1;                  /* T(1,0) = 1 */
        } else if (r == 2) {
            tri_cache[start]     = (seq_t)0;              /* T(2,0) = 0 */
            tri_cache[start + 1] = (seq_t)2;              /* T(2,1) = 2 */
        } else {
            size_t prev1 = row_start(r - 1);
            size_t prev2 = row_start(r - 2);

            for (size_t k = 0; k < r; ++k) {
                seq_t val = 0;

                /* T(r-1,k) */
                if (k < r - 1)
                    val += tri_cache[prev1 + k];

                /* T(r-1,k-1) */
                if (k > 0)
                    val += tri_cache[prev1 + k - 1];

                /* -T(r-2,k-1) */
                if (k > 0)
                    val -= tri_cache[prev2 + k - 1];

                /* +2*T(r-2,k-2) */
                if (k >= 2)
                    val += (seq_t)2 * tri_cache[prev2 + k - 2];

                tri_cache[start + k] = val;
            }
        }
    }
    rows_done = target;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined below the offset */

    /* determine the row containing the n-th flattened entry */
    size_t nn   = (size_t)n;
    size_t row  = 1;
    while ((row * (row + 1)) / 2 < nn)
        ++row;

    /* make sure the triangle has been computed up to that row */
    ensure_rows(row);

    /* locate the exact element */
    size_t start  = row_start(row);
    size_t offset = nn - start - 1;  /* 0‑based offset within the row */

    return tri_cache[start + offset];
}
