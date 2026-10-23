/*
 * A027023  Tribonacci array: triangular array T read by rows: T(n,0)=1 for n
 *          >= 0, T(n,1) = T(n,2n) = 1 for n >= 1, T(n,2)=1 for n >= 2 and for
 *          n >= 3, T(n,k) = T(n-1,k-3) + T(n-1,k-2) + T(n-1,k-1) for 3 <= k
 *          <= 2n-1.
 * https://oeis.org/A027023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, nice
 * Formula:   a(n) = T(⌊√n⌋, n – ⌊√n⌋²), where T is the triangular Tribonacci
 *            array: T(0,0) = 1 T(n,0) = 1 T(n,1) = 1 for n ≥ 1 T(n,2n) = 1
 *            for n ≥ 1 T(n,2) = 1 for n ≥ 2 T(n,k) =
 *            T(n-1,k-3)+T(n-1,k-2)+T(n-1,k-1) for 3 ≤ k ≤ 2n‑1
 * Range:     u8 n <= 74, u16 n <= 189, u32 n <= 615, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>   /* for uint64_t, if needed */

static seq_t **rows = NULL;
static size_t rows_count = 0;

/* integer square‑root, safe for 64‑bit inputs */
static idx_t int_sqrt(idx_t n)
{
    idx_t r = (idx_t)sqrt((double)n);
    while ((unsigned __int128)(r + 1) * (r + 1) <= n) ++r;
    while ((unsigned __int128)r * r > n) --r;
    return r;
}

/* allocate and fill all rows up to and including 'up_to' */
static void ensure_rows(idx_t up_to)
{
    if (rows_count > up_to)
        return;

    rows = (seq_t **)realloc(rows, (up_to + 1) * sizeof(seq_t *));
    if (!rows) exit(EXIT_FAILURE);

    for (idx_t i = rows_count; i <= up_to; ++i)
    {
        size_t len = 2 * i + 1;                       /* number of columns in row i */
        seq_t *row = (seq_t *)calloc(len, sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);

        /* base values */
        row[0] = (seq_t)1;               /* T(i,0) */
        if (i >= 1) {
            row[1]     = (seq_t)1;         /* T(i,1) */
            row[len-1] = (seq_t)1;         /* T(i,2i) */
        }
        if (i >= 2) {
            row[2] = (seq_t)1;             /* T(i,2) */
        }

        /* fill interior using the Tribonacci recurrence */
        if (i >= 2) {
            seq_t *prev = rows[i - 1];     /* previous row has length len-2 */
            for (size_t k = 3; k <= len - 2; ++k) {
                row[k] = prev[k - 3] + prev[k - 2] + prev[k - 1];
            }
        }

        rows[i] = row;
    }

    rows_count = up_to + 1;
}

seq_t a(idx_t n)
{
    idx_t r = int_sqrt(n);
    idx_t k = n - r * r;      /* offset within the row */
    ensure_rows(r);
    return rows[r][k];
}
