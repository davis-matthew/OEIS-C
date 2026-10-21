/*
 * A058394  A square array based on natural numbers (A000027) with each term
 *          being the sum of 2 consecutive terms in the previous row.
 * https://oeis.org/A058394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   diag_start
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A058394 : a square array based on natural numbers.
   T(n,k) = T(n-1,k-1)+T(n,k-1) for n,k>0
   Boundary : T(0,k)=1,  T(2n,0)=T(n,2),  T(2n+1,0)=0
   a(n) is the anti‑diagonal enumeration of T(n,k).
   The function handles arbitrary seq_t (unsigned integer or BigInt).
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/diag_start.h"

/* 2. Cached table for T(n,k) up to rows/cols = s_max+1. */
static seq_t *table      = NULL;   /* 1‑D array indexed as [i*cols + j] */
static idx_t  table_smax = 0;      /* maximum row currently stored   */
static size_t table_cols = 0;      /* = table_smax + 1 */

/* 3. Build or enlarge the table up to new_smax (inclusive). */
static void rebuild_table(idx_t new_smax)
{
    size_t cols = new_smax + 1;
    size_t size = cols * cols;
    seq_t *new_t = (seq_t *)calloc(size, sizeof(seq_t));
    if (!new_t) return;               /* allocation failure -> keep old */

    /* Row 0: T(0,k)=1 for all k. */
    for (idx_t j = 0; j <= new_smax; ++j)
        new_t[j] = (seq_t)1;

    /* Remaining rows. */
    for (idx_t i = 1; i <= new_smax; ++i) {
        /* First column (k=0). */
        if ((i & 1) == 0) {           /* even i: T(2n,0)=T(n,2) */
            idx_t n = i >> 1;
            new_t[i * cols + 0] = new_t[n * cols + 2];
        } else {                      /* odd i: T(2n+1,0)=0 */
            new_t[i * cols + 0] = (seq_t)0;
        }

        /* Remaining columns (k>0). */
        for (idx_t j = 1; j <= new_smax; ++j) {
            seq_t a = new_t[(i - 1) * cols + (j - 1)];
            seq_t b = new_t[i * cols + (j - 1)];
            new_t[i * cols + j] = a + b;
        }
    }

    /* Replace old table. */
    if (table) free(table);
    table      = new_t;
    table_smax = new_smax;
    table_cols = cols;
}

/* 4. Public function. */
seq_t a(idx_t n)
{
    /* diag_start gives the anti‑diagonal index s and offset within it. */
    idx_t s       = diag_start(n);
    idx_t before  = (idx_t)((unsigned __int128)s * (s + 1) / 2);
    idx_t offset  = n - before;          /* 0 ≤ offset ≤ s */
    idx_t i = s - offset;                /* row index   */
    idx_t j = offset;                    /* column index */

    /* Ensure the table covers this row/column. */
    if (s >= table_smax || table == NULL)
        rebuild_table(s);

    return table[i * table_cols + j];
}
