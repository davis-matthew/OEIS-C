/*
 * A014459  Odd numbers in the triangle of Eulerian numbers.
 * https://oeis.org/A014459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 52, u64 n <= 125,
 *            u128 n <= 298, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th odd entry in the triangle of Eulerian numbers
   (row-wise, left-to-right, 1-indexed).  The function computes rows
   incrementally, collecting odd entries until the requested index is
   reached.  It works for the built-in integer types as well as the
   arbitrary‑precision class used by the big integer build. */

#include <stddef.h>   /* for size_t */

/* Maximum row length needed for n up to ~10000 (adjust if higher n is required).
   5000 gives plenty of room for all rows that may be generated. */
#define MAX_ROW 5000
/* Allocate storage to hold all odd entries that may be needed.  20000 is safe for n≤10000. */
#define MAX_ODD 20000

/* Static storage for two consecutive rows of Eulerian numbers. */
static seq_t prev_row[MAX_ROW];
static seq_t cur_row[MAX_ROW];

/* Storage for the odd entries discovered so far. */
static seq_t odd_vals[MAX_ODD];

/* Current state of the incremental generation. */
static size_t generated_rows = 0; /* number of rows fully processed (row 1 corresponds to index 1) */
static size_t odd_cnt       = 0; /* number of odd entries stored */

/* Flag to initialise the static arrays once. */
static int initialized = 0;

seq_t a(idx_t n)
{
    /* Index 0 is outside the offset; anything less than 1 is undefined,
       but we return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise on first call. */
    if (!initialized) {
        /* Row 1: single entry 1. */
        prev_row[0] = (seq_t)1;
        odd_vals[0] = (seq_t)1;
        generated_rows = 1;
        odd_cnt = 1;
        initialized = 1;
    }

    /* Generate rows until we have collected at least n odd entries. */
    while (odd_cnt < (size_t)n) {
        size_t r = generated_rows + 1;            /* next row number (1‑based) */
        size_t len = r;                          /* number of elements in row r */

        if (len > MAX_ROW)
            return (seq_t)0; /* safety: not enough space, should not happen for n<=10000 */

        /* First and last entries are always 1. */
        cur_row[0] = cur_row[len - 1] = (seq_t)1;

        /* Intermediary entries computed via recurrence: */
        /* A(n,k) = (k+1)*A(n-1,k) + (n-k)*A(n-1,k-1) */
        for (size_t k = 1; k + 1 < len; ++k) {
            seq_t term1 = ((seq_t)(k + 1)) * prev_row[k];
            seq_t term2 = ((seq_t)(len - k)) * prev_row[k - 1];
            cur_row[k] = term1 + term2;
        }

        /* Collect odd elements of this row, in left‑to‑right order. */
        for (size_t i = 0; i < len; ++i) {
            if ((cur_row[i] & (seq_t)1) != (seq_t)0) {
                if (odd_cnt >= MAX_ODD)
                    return (seq_t)0; /* safety overflow */
                odd_vals[odd_cnt++] = cur_row[i];
            }
        }

        /* Prepare for next iteration: swap prev_row and cur_row. */
        for (size_t i = 0; i < len; ++i)
            prev_row[i] = cur_row[i];

        ++generated_rows;
    }

    /* Return the requested odd entry.  n is 1‑based, array is 0‑based. */
    return odd_vals[n - 1];
}
