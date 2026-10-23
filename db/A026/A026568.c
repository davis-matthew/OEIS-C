/*
 * A026568  Irregular triangular array T read by rows: T(i,0) = T(i,2i) = 1
 *          for i >= 0; T(i,1) = T(i,2i-1) = [ (i+1)/2 ] for i >= 1; and for i
 *          >= 2 and 2 <=j <= i - 2, T(i,j) = T(i-1,j-2) + T(i-1,j-1) +
 *          T(i-1,j) if i + j is even, T(i,j) = T(i-1,j-2) + T(i-1,j) if i + j
 *          is odd.
 * https://oeis.org/A026568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the triangular array read by rows (OEIS A026568). The
 *            value T(i,j) is computed by recurrence: T(i,0) = T(i,2i) = 1
 *            T(i,1) = T(i,2i-1) = floor((i+1)/2) for 2 ≤ j ≤ 2i-2 if (i+j)
 *            even: T(i,j) = T(i‑1,j‑2)+T(i‑1,j‑1)+T(i‑1,j) else : T(i,j) =
 *            T(i‑1,j‑2)+T(i‑1,j)
 * Range:     u8 n <= 70, u16 n <= 208, u32 n <= 702, u64 n <= 2548,
 *            u128 n <= 9694, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer square root: floor(sqrt(x)) via binary search. */
static inline idx_t int_sqrt(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Cached rows of the triangular array. */
static seq_t **rows = NULL;      /* pointer to each row array */
static idx_t   rows_cap = 0;     /* number of rows allocated */

/* Ensure that rows[0..i] are computed and stored. */
static void ensure_rows(idx_t i)
{
    /* Allocate space for more rows if needed. */
    if (i < rows_cap) return;
    /* Grow rows array. */
    size_t new_cap = i + 1;
    seq_t **new_rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
    if (!new_rows) return; /* allocation failure: subsequent use will fail */
    rows = new_rows;
    rows_cap = new_cap;

    /* Compute any missing rows. */
    for (idx_t k = 0; k <= i; ++k) {
        if (rows[k]) continue;          /* already computed */
        idx_t len = 2 * k + 1;           /* number of elements in row k */
        seq_t *row = (seq_t *)calloc((size_t)len, sizeof(seq_t));
        if (!row) continue;             /* allocation failure */
        /* Boundary values. */
        row[0] = 1;
        row[len - 1] = 1;
        if (k >= 1) {
            row[1] = row[len - 2] = (seq_t)((k + 1) / 2);
        }
        /* Interior values. */
        if (k >= 2) {
            for (idx_t j = 2; j <= len - 3; ++j) {
                if ((k + j) & 1u) {        /* odd parity: two terms */
                    row[j] = rows[k - 1][j - 2] + rows[k - 1][j];
                } else {                   /* even parity: three terms */
                    row[j] = rows[k - 1][j - 2]
                           + rows[k - 1][j - 1]
                           + rows[k - 1][j];
                }
            }
        }
        rows[k] = row;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined, but safe */

    /* Find row i such that i^2 < n <= (i+1)^2. */
    idx_t i = int_sqrt(n - 1);

    ensure_rows(i);
    if (!rows[i]) return 0;            /* allocation failure */

    idx_t offset = n - i * i - 1;      /* column within row i */
    return rows[i][offset];
}
