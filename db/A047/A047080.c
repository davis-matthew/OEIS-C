/*
 * A047080  Triangular array T read by rows: T(h,k)=number of paths from (0,0)
 *          to (k,h-k) using step-vectors (0,1), (1,0), (1,1) with no right
 *          angles between pairs of consecutive steps.
 * https://oeis.org/A047080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 261, u32 n <= 877, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A047080
   Triangular array T read by rows:
     T(h,k) = number of paths from (0,0) to (k, h-k) using steps
               (0,1), (1,0), (1,1) with no right angles between consecutive steps.
   The sequence a(n) is obtained by flattening this triangle by rows.
   Recurrence (valid for h ≥ 4):
     T(h,k) = T(h-1,k-1) + T(h-1,k) - T(h-4,k-2), with T(…,−1)=T(−1,…) = 0.
   Base rows:
     T(0,0)=1;
     T(1,0)=T(1,1)=1;
     T(2,0)=T(2,1)=T(2,2)=1;
     T(3,0)=T(3,3)=1;  T(3,1)=T(3,2)=2.
   The function below builds only the needed rows and caches them
   between calls.  All computations use only integer operations
   allowed for seq_t and idx_t. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Find the row index h that contains the n‑th flattened term. */
    idx_t h = 0;
    while ((h + 1) * (h + 2) / 2 <= n) {
        ++h;
    }

    /* Column index k in row h.  Each row h has length h+1. */
    idx_t k = n - h * (h + 1) / 2;   /* start of row h is h*(h+1)/2 */

    /* --- cached triangle ------------------------------------------- */
    /* cached_row is the largest row that has already been computed.
       cached points to a square array of size (cached_row+1) × (cached_row+1)
       that stores all T values that were computed up to that row. */
    static idx_t cached_row = (idx_t)-1;   /* sentinel: nothing cached yet */
    static seq_t *cached = NULL;

    if (cached == NULL || h > cached_row) {
        /* Recompute the triangle up to the required row h. */
        if (cached) {
            free(cached);
            cached = NULL;
        }

        idx_t maxh = h;
        size_t dim   = (size_t)maxh + 1;         /* array dimension */
        size_t arrlen = dim * dim;               /* number of elements */

        cached = (seq_t*)malloc(arrlen * sizeof(seq_t));
        if (cached == NULL) return (seq_t)0;     /* memory failure */

        /* initialise all entries to 0 */
        memset(cached, 0, arrlen * sizeof(seq_t));

        /* base rows */
        cached[0 * dim + 0] = 1;
        if (maxh >= 1) {
            cached[1 * dim + 0] = 1;
            cached[1 * dim + 1] = 1;
        }
        if (maxh >= 2) {
            cached[2 * dim + 0] = 1;
            cached[2 * dim + 1] = 1;
            cached[2 * dim + 2] = 1;
        }
        if (maxh >= 3) {
            cached[3 * dim + 0] = 1;
            cached[3 * dim + 1] = 2;
            cached[3 * dim + 2] = 2;
            cached[3 * dim + 3] = 1;
        }

        /* compute rows h ≥ 4 */
        for (idx_t hh = 4; hh <= maxh; ++hh) {
            for (idx_t kk = 0; kk <= hh; ++kk) {
                seq_t val = 0;
                if (kk > 0)
                    val += cached[(hh - 1) * dim + (kk - 1)];
                val += cached[(hh - 1) * dim + kk];
                if (kk >= 2)
                    val -= cached[(hh - 4) * dim + (kk - 2)];
                cached[hh * dim + kk] = val;
            }
        }

        cached_row = maxh;   /* remember the largest row cached */
    }

    /* Return the requested element from the cached triangle. */
    size_t dim = (size_t)cached_row + 1;
    return cached[h * dim + k];
}
