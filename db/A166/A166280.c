/*
 * A166280  Stirling2 triangle mod 2, T(n,k) = A008277(n,k) mod 2.
 * https://oeis.org/A166280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166280: Stirling‑2 triangle mod 2.
 *
 * a(n) = S(n+1, k+1) mod 2 where n is the flattened (offset‑0) index,
 * k = n – n*(n+1)/2.  The triangle consists of rows
 *   row r (0‑based) : S(r+1, k+1)  for k = 0 … r
 * with length r + 1.
 *
 * Each entry is computed modulo 2 using the recurrence
 *   S(m, k) = k S(m‑1, k) + S(m‑1, k‑1)  (mod 2).
 * Since k mod 2 is 1 only for odd k, the formula reduces to
 *
 *   S(m, k) mod 2 = (k % 2) ? S(m‑1, k) : 0  XOR  S(m‑1, k‑1) mod 2.
 *
 * The implementation keeps the computed rows in static cache so that
 * calls to a() in any order are fast.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Find row r and column k (0‑based) for the flattened index n.
    ------------------------------------------------------------------ */
    idx_t r = 0;
    idx_t remaining = n;

    /* Row r has length r+1. */
    while (1) {
        idx_t len = r + 1;
        if (remaining >= len) {
            remaining -= len;
            ++r;
        } else
            break;
    }
    idx_t k = remaining; /* 0‑based column inside row r */

    /* ------------------------------------------------------------------
       Lazy initialisation of the cache of rows.
    ------------------------------------------------------------------ */
    static seq_t **rows = NULL;   /* rows[i] points to an array of length i+1 */
    static idx_t cached = 0;      /* number of rows stored */

    if (cached == 0) {
        /* Allocate space for the first two rows (r = 0 and r = 1). */
        rows = (seq_t **)malloc(2 * sizeof(seq_t *));
        if (!rows) abort();

        /* Row 0 : length 1, S(1,1)=1 */
        rows[0] = (seq_t *)malloc(1 * sizeof(seq_t));
        if (!rows[0]) abort();
        rows[0][0] = (seq_t)1;
        cached = 1;

        /* Row 1 : length 2, S(2,1)=S(2,2)=1 */
        rows[1] = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!rows[1]) abort();
        rows[1][0] = (seq_t)1;
        rows[1][1] = (seq_t)1;
        cached = 2;
    }

    /* ------------------------------------------------------------------
       Grow the cache to include row r if necessary.
    ------------------------------------------------------------------ */
    if (cached <= r) {
        /* Ensure array of pointers has enough capacity.  Use double size
           strategy for amortised linear growth. */
        size_t ptr_cap = (size_t)(cached > 2 ? 2 * cached : cached + 2);
        while ((idx_t)ptr_cap <= r) ptr_cap *= 2;
        if ((size_t)cached < ptr_cap) {
            seq_t **tmp = (seq_t **)realloc(rows, ptr_cap * sizeof(seq_t *));
            if (!tmp) abort();
            rows = tmp;
        }

        /* Build new rows from 'cached' up to 'r'. */
        for (idx_t i = cached; i <= r; ++i) {
            idx_t len = i + 1;
            seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
            if (!row) abort();

            /* Base cases: S(i+1,1) = 1 for all i, and S(i+1,i+1) = 1. */
            row[0] = (seq_t)1;
            if (len > 1) row[len - 1] = (seq_t)1;

            /* Fill interior entries using the parity recurrence. */
            if (i >= 2) { /* rows with length >= 3 need interior cells */
                seq_t *prev = rows[i - 1]; /* previous row of length i */
                for (idx_t j = 1; j + 1 < len; ++j) {
                    /* k = j+1 (1‑based).  k is odd iff (j+1)&1 == 1. */
                    seq_t left  = (( (j + 1) & 1 ) ? prev[j] : (seq_t)0);
                    seq_t right = prev[j - 1];
                    row[j] = left ^ right;
                }
                /* j == len-1 case (k = i+1) has left = 0, right = prev[i-1],
                   already set to 1 by the base case. */
            }

            rows[i] = row;
        }
        cached = r + 1;
    }

    /* ------------------------------------------------------------------
       Return the requested parity entry.  k is 0‑based column within row r.
    ------------------------------------------------------------------ */
    return rows[r][k];
}
