/*
 * A129714  Triangle read by rows: T(n,k) is the number of Fibonacci binary
 *          words of length n and having k runs (0<=k<=n). A Fibonacci binary
 *          word is a binary word having no 00 subword. A run is a maximal
 *          sequence of consecutive identical letters.
 * https://oeis.org/A129714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n-th element of the flattened triangle T(n,k) where
 *            T(n,k) counts Fibonacci binary words of length n having exactly
 *            k runs of identical letters. The triangle is defined by the
 *            recurrence T(0,0) = 1 T(1,0) = 0 , T(1,1) = 2 T(2,0) = 0 ,
 *            T(2,1) = 1 , T(2,2) = 2 and for n >= 3 T(n,k) = T(n...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   row_cumulative
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/row_cumulative.h"

/* ----- helper functions ----- */

static idx_t find_row(idx_t idx)
{
    /* return the smallest r such that row_cumulative(r) > idx
       which is the row containing the flattened index idx. */
    idx_t low = 0;
    idx_t high = 1;
    while (row_cumulative(high) <= idx)
        high <<= 1;                 /* exponential search */
    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        if (row_cumulative(mid) > idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;                     /* low == high */
}

/* ----- cached triangle storage ----- */

static idx_t cached_rows = 0;   /* number of allocated row pointers */
static seq_t **rows = NULL;     /* pointer to array of row pointers */

/* ensure that rows[0..needed] exist, allocating new rows as necessary */
static void ensure_rows(idx_t needed)
{
    if (cached_rows <= needed)
    {
        idx_t new_cap = cached_rows ? cached_rows << 1 : 1;
        while (new_cap <= needed) new_cap <<= 1;

        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp) abort();           /* memory error */
        rows = tmp;
        for (idx_t i = cached_rows; i < new_cap; ++i)
            rows[i] = NULL;          /* mark uninitialized */
        cached_rows = new_cap;
    }

    /* compute rows that are missing, in order */
    for (idx_t n = 0; n <= needed; ++n)
    {
        if (rows[n]) continue;     /* already computed */

        idx_t size = n + 1;
        seq_t *row = (seq_t*)malloc(size * sizeof(seq_t));
        if (!row) abort();

        if (n == 0)
        {
            row[0] = (seq_t)1;
        }
        else if (n == 1)
        {
            row[0] = (seq_t)0;
            row[1] = (seq_t)2;
        }
        else if (n == 2)
        {
            row[0] = (seq_t)0;
            row[1] = (seq_t)1;
            row[2] = (seq_t)2;
        }
        else
        {
            /* recurrence: T(n,k) = T(n-1,k) + (k>=2 ? T(n-2,k-2) : 0) */
            seq_t *row1 = rows[n - 1];
            seq_t *row2 = rows[n - 2];
            for (idx_t k = 0; k <= n; ++k)
            {
                seq_t val = (k <= n - 1 ? row1[k] : (seq_t)0);
                if (k >= 2)
                    val += row2[k - 2];
                row[k] = val;
            }
        }
        rows[n] = row;
    }
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    /* mapping from flattened index n to triangle position (row, col) */
    idx_t r = find_row(n);
    idx_t prev_cum = (r == 0) ? 0 : row_cumulative(r - 1);
    idx_t k = n - prev_cum;

    ensure_rows(r);
    return rows[r][k];
}
