/*
 * A216054  Square array T, read by antidiagonals: T(n,k) = 0 if n-k >= 1 or
 *          if k-n >= 6, T(0,0) = T(0,1) = T(0,2) = T(0,3) = T(0,4) = T(0,5) =
 *          1, T(n,k) = T(n-1,k) + T(n,k-1).
 * https://oeis.org/A216054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k) = T(n-1,k) + T(n,k-1);
   Base:  T(0,k) = 1  for k = 0 … 5
          T(n,k) = 0   if n > k  (n-k ≥ 1) or k > n+5  (k-n ≥ 6).
   Sequence a(n) is obtained by reading the array T along antidiagonals
   (i + j = constant).  The first terms are:
   a(0)=1, a(1)=1, a(2)=0, a(3)=1, a(4)=1, a(5)=0, …               */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for the band‑limited triangular array T.
   Each row r stores values for k = r + offset, offset = 0 … 5. */
static seq_t **T_table = NULL;
static size_t  T_rows  = 0;          /* number of rows currently allocated */

/* Ensure that rows 0 … needed are computed. */
static void ensure_rows(size_t needed)
{
    if (needed < T_rows)                    /* nothing to do */
        return;

    size_t old_rows = T_rows;
    size_t new_rows = needed + 1;

    /* reallocate the row‑pointer array */
    T_table = (seq_t **)realloc(T_table, new_rows * sizeof(seq_t *));
    /* allocate new rows and initialise them to 0 */
    for (size_t r = old_rows; r < new_rows; ++r) {
        T_table[r] = (seq_t *)malloc(6 * sizeof(seq_t));
        for (size_t c = 0; c < 6; ++c)
            T_table[r][c] = (seq_t)0;      /* zero initialisation */
    }

    /* initialise row 0 with ones only once */
    if (T_rows == 0) {
        for (size_t c = 0; c < 6; ++c)
            T_table[0][c] = (seq_t)1;
    }

    /* compute rows 1 … needed */
    for (size_t r = 1; r <= needed; ++r) {
        for (size_t offset = 0; offset <= 5; ++offset) {
            seq_t left = (offset > 0) ? T_table[r][offset - 1] : (seq_t)0;
            seq_t up   = (offset < 5) ? T_table[r - 1][offset + 1] : (seq_t)0;
            T_table[r][offset] = left + up;
        }
    }

    T_rows = new_rows;
}

/* Return the nth term of sequence A216054. */
seq_t a(idx_t n)
{
    /* Locate the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    size_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= (size_t)n)
        ++d;

    size_t diag_start = d * (d + 1) / 2;    /* first index on this diagonal */
    size_t offset     = (size_t)n - diag_start;   /* offset on the diagonal */

    size_t i = offset;          /* row index */
    size_t j = d - offset;      /* column index */

    /* Out of band positions are zero. */
    if (i > j)                /* n-k >= 1  → k < n  */
        return (seq_t)0;
    if (j > i + 5)            /* k-n >= 6  → column too far to the right */
        return (seq_t)0;

    /* Ensure the needed rows are available. */
    ensure_rows(d);

    /* Return the stored value.  Off‑by‑one offset maps k-i. */
    return T_table[i][j - i];
}
