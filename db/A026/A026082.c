/*
 * A026082  Irregular triangular array T read by rows: T(n,k) = C(n,k) for
 *          k=0..n for n = 0,1,2,3. For n >= 4, T(n,0) = T(n,2n)=1, T(n,1) =
 *          T(n,2n-1) = n - 3, T(4,2) = 4, T(4,3) = 3, T(4,4) = 6; T(4,5) = 3,
 *          T(4,6)=4; for n >= 5, T(n,k) = T(n-1,k-2) + T(n-1,k-1) + T(n-1,k)
 *          for k=2..2n-2.
 * https://oeis.org/A026082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 200, u32 n <= 590, u64 n <= 1972,
 *            u128 n <= 7297, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(1..∞) defined from the irregular triangular array T(n,k) (OEIS A026082).
   The array is read row by row and each element of the rows forms the sequence.
   Recurrence for n ≥ 5:
        T(n,0)      = T(n,2n)      = 1
        T(n,1)      = T(n,2n-1)    = n-3
        T(n,k)      = T(n-1,k-2) + T(n-1,k-1) + T(n-1,k)   for 2 ≤ k ≤ 2n-2
   Special rows for n = 0..4 are given explicitly. */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free */

/* C++ compatibility: use new/delete for non‑trivial types, otherwise malloc/free. */
#if defined(__cplusplus)
#  define USE_NEW 1
#else
#  define USE_NEW 0
#endif

static inline seq_t *alloc_row(idx_t len)
{
#if USE_NEW
    return new seq_t[len];
#else
    return (seq_t *)malloc(len * sizeof(seq_t));
#endif
}

static inline void free_row(seq_t *row)
{
#if USE_NEW
    delete[] row;
#else
    free(row);
#endif
}

seq_t a(idx_t n)
{
    if (n < 1)                            /* safety for undefined region */
        return (seq_t)0;                  /* not needed by the specification */

    /* locate which row and index within that row contain a(n) */
    idx_t row = 0, cum = 0;
    while (1) {
        idx_t len   = (row <= 3) ? (row + 1) : (2 * row + 1);
        if (cum + len >= n)
            break;
        cum += len;
        ++row;
    }
    idx_t pos = n - cum - 1;              /* zero‑based position in the row */

    /* build rows up to 'row', keeping only the previous row in memory */
    seq_t *prev = NULL;                   /* previous row (NULL for row 0) */

    for (idx_t r = 0; r <= row; ++r) {
        idx_t len = (r <= 3) ? (r + 1) : (2 * r + 1);
        seq_t *cur = alloc_row(len);

        /* fill 'cur' according to the definition */
        if (r == 0) {
            cur[0] = (seq_t)1;
        } else if (r == 1) {
            cur[0] = cur[1] = (seq_t)1;
        } else if (r == 2) {
            cur[0] = (seq_t)1;
            cur[1] = (seq_t)2;
            cur[2] = (seq_t)1;
        } else if (r == 3) {
            cur[0] = (seq_t)1;
            cur[1] = (seq_t)3;
            cur[2] = (seq_t)3;
            cur[3] = (seq_t)1;
        } else if (r == 4) {                       /* special row */
            cur[0] = (seq_t)1; cur[1] = (seq_t)1; cur[2] = (seq_t)4;
            cur[3] = (seq_t)3; cur[4] = (seq_t)6; cur[5] = (seq_t)3;
            cur[6] = (seq_t)4; cur[7] = (seq_t)1; cur[8] = (seq_t)1;
        } else {                                 /* r >= 5, use recurrence */
            for (idx_t k = 0; k < len; ++k) {
                if (k == 0 || k == len - 1) {
                    cur[k] = (seq_t)1;
                } else if (k == 1 || k == len - 2) {
                    cur[k] = (seq_t)(r - 3);          /* n - 3 */
                } else {
                    cur[k] = prev[k - 2] + prev[k - 1] + prev[k];
                }
            }
        }

        /* if this is the target row, extract the value and clean up */
        if (r == row) {
            seq_t result = cur[pos];
            free_row(prev);
            free_row(cur);
            return result;
        }

        /* move to next iteration: current row becomes previous row */
        free_row(prev);
        prev = cur;
    }

    /* control never reaches here, but keep compiler happy */
    free_row(prev);
    return (seq_t)0;
}
