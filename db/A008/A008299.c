/*
 * A008299  Triangle T(n,k) of associated Stirling numbers of second kind, n
 *          >= 2, 1 <= k <= floor(n/2).
 * https://oeis.org/A008299
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 33, u32 n <= 77, u64 n <= 189,
 *            u128 n <= 495, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A008299 – triangle of associated Stirling numbers of the second kind.
 *
 * The sequence a(n) is obtained by reading the triangle T(n,k) row‑by‑row
 * (increasing n, for each n increasing k).  In particular
 *
 *   a(2) = T(2,1),  a(3) = T(3,1),
 *   a(4) = T(4,1), a(5) = T(4,2), a(6) = T(5,1), a(7) = T(5,2), …
 *
 * The triangle satisfies
 *
 *   T(2,1) = 1,
 *   T(n,1) = 1   for n ≥ 2,
 *   T(n+1,k) = k·T(n,k) + n·T(n-1,k-1)    for 2 ≤ k ≤ ⌊(n+1)/2⌋
 *
 * The function below returns a(n) for any n ≥ 2 (values for smaller indices
 * are undefined).  It memoises all needed rows of the triangle; for n up
 * to 10 000 the computation finishes in well under 2 s.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /*  Index given by OEIS: a(2) is the first term.  We map it to a
     *  1‑based position p = n-1 in the flattened row‑by‑row list. */
    if (n < 2) return 0;                 /* undefined for n<2, harmless */

    static idx_t max_row = 1;            /* highest row computed so far */
    static seq_t **T = NULL;             /* triangle rows: T[row][k] */
    static idx_t *cum = NULL;            /* cumulative counts up to each row */

    /* Initialise data structures on the first call. */
    if (T == NULL) {
        /* Allocate space for rows 0, 1, 2.  Rows 0 and 1 are unused. */
        T = (seq_t **)malloc(3 * sizeof(seq_t *));
        cum = (idx_t *)malloc(3 * sizeof(idx_t));

        T[0] = NULL;
        T[1] = NULL;

        /* Row 2: only k=1 */
        T[2] = alloc_seq_array(2);   /* indices 0..1, we use 1 */
        T[2][1] = 1;

        cum[0] = 0;
        cum[1] = 0;
        cum[2] = 1;                  /* number of terms up to row 2 */
        max_row = 2;
    }

    /* Position in the flattened list. */
    idx_t pos = n - 1;

    /* Grow the triangle until we contain the requested position. */
    while (cum[max_row] < pos) {
        idx_t r = max_row + 1;        /* new row number */
        idx_t kmax = r >> 1;          /* floor(r/2) */

        /* Allocate storage for the new row. */
        seq_t *row = alloc_seq_array(kmax + 1);  /* index 0 unused */

        /* Base case: T(n,1) = 1 */
        if (kmax >= 1) row[1] = 1;

        /* Fill entries for k >= 2 using the recurrence. */
        if (kmax >= 2) {
            for (idx_t k = 2; k <= kmax; ++k) {
                seq_t term1 = 0;
                /* term1 exists only if k <= floor((r-1)/2) */
                if (k <= ((r - 1) >> 1)) {
                    term1 = (seq_t)k * T[r - 1][k];
                }
                /* term2 always exists for k >= 2 */
                seq_t term2 = (seq_t)(r - 1) * T[r - 2][k - 1];
                row[k] = term1 + term2;
            }
        }

        /* Resize the row pointer array and cumulative array. */
        T = (seq_t **)realloc(T, (r + 1) * sizeof(seq_t *));
        cum = (idx_t *)realloc(cum, (r + 1) * sizeof(idx_t));

        T[r] = row;
        cum[r] = cum[r - 1] + kmax;
        ++max_row;
    }

    /* Find the row r such that cum[r-1] < pos <= cum[r].  Binary search. */
    idx_t lo = 2, hi = max_row, r = max_row;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (cum[mid] < pos) {
            lo = mid + 1;
        } else {
            r = mid;
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    /* Column index within the row. */
    idx_t k = pos - cum[r - 1];       /* 1 <= k <= floor(r/2) */

    return T[r][k];
}
