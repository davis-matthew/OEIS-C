/*
 * A132005  Triangle, read by rows, where T(n,k) = n*T(n-1,k-1) + T(n-1,k-2)
 *          for n>0 and k>1, with T(n,0) = T(n-1,n-1) and T(n,1) = n*T(n-1,0)
 *          for n>0 and T(0,0) = 1.
 * https://oeis.org/A132005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 42, u32 n <= 98, u64 n <= 237,
 *            u128 n <= 621, big any n
 * Helpers:   alloc_seq_array, cast_idx, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows, as defined in OEIS A132005.
   The sequence a(n) is the n-th element of the flattened triangle.
   Triangle definition:   T(n,0)=T(n-1,n-1), T(n,1)=n*T(n-1,0),
   T(n,k)=n*T(n-1,k-1)+T(n-1,k-2)   (n>0, k>1).  T(0,0)=1. */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>

/* Utility headers for dynamic allocation and the constant one */
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_one.h"
#include "../utils/cast_idx.h"

/* Cache of computed rows.  triangle_cache[r][k] holds T(r,k). */
static seq_t **triangle_cache = NULL;   /* array of pointers to rows */
static idx_t cached_rows = 0;           /* number of rows currently stored (0 … cached_rows-1) */

/* Ensure that rows 0 … target are computed and stored in triangle_cache. */
static void ensure_rows(idx_t target)
{
    if (target < cached_rows) return;

    /* Resize the array of row pointers if necessary */
    if (triangle_cache == NULL) {
        triangle_cache = (seq_t**)malloc(sizeof(seq_t*) * (target + 1));
    } else {
        triangle_cache = (seq_t**)realloc(triangle_cache, sizeof(seq_t*) * (target + 1));
    }

    /* Compute any new rows */
    for (idx_t r = cached_rows; r <= target; ++r) {
        triangle_cache[r] = alloc_seq_array(r + 1);   /* allocate space for the row */

        if (r == 0) {
            triangle_cache[0][0] = seq_one();        /* T(0,0)=1 */
        } else {
            seq_t *prev = triangle_cache[r - 1];
            seq_t *cur  = triangle_cache[r];

            cur[0] = prev[r - 1];                    /* T(r,0) = T(r-1,r-1) */
            cur[1] = cast_idx(r) * prev[0];         /* T(r,1) = r*T(r-1,0) */

            for (idx_t k = 2; k <= r; ++k) {
                cur[k] = cast_idx(r) * prev[k - 1] + prev[k - 2];  /* recurrence */
            }
        }
    }

    cached_rows = target + 1;
}

/* Binary search to locate the row r containing the global index n.
   We find the largest r such that r*(r+1)/2 <= n. */
static idx_t row_of(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;       /* upper mid */
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function.  External linkage as required. */
seq_t a(idx_t n)
{
    idx_t r   = row_of(n);                                 /* row index */
    ensure_rows(r);                                        /* make sure the row exists */

    /* start of row r: number of elements before this row */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;                                   /* column index */

    return triangle_cache[r][k];                          /* T(r,k) */
}
