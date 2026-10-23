/*
 * A131791  Triangle read by rows of 2^n terms for n>=0: let S(n) denote the
 *          initial 2^n terms of the partial sums of row n; starting with a
 *          single '1' in row 0, generate row n+1 by concatenating S(n) with
 *          the terms of S(n) when read in reverse order.
 * https://oeis.org/A131791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 155, u32 n <= 1105, u64 n <= 2046,
 *            u128 n <= 2046, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A131791
   The sequence a(n) is the nth entry of a triangle whose rows are
   built as follows:
   • Row 0 has a single 1.
   • For r ≥ 1, the first half of row r is the cumulative (partial) sums
     of row r‑1; the second half is a reverse copy of that first half.
   The global index n traverses the rows in order, so the first index
   of row r is 2^r‑1 and the row contains 2^r values. */

/* Necessary standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of the user‑supplied types */
typedef uint64_t idx_t;          /* index type supplied by the build */
                                   /* never define seq_t here – it is defined by the build */

/* Utility: floor(log2(x)) for a 64‑bit unsigned integer
   (x > 0).  The loop is fine for the small values we need. */
static inline idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Cache of generated rows:
   rows[r] points to an array of 2^r elements of type seq_t. */
static seq_t **rows      = NULL;
static idx_t   rows_count = 0;   /* number of rows currently stored */

/* Expand the cache to contain all rows up to and including 'desired_row'. */
static void ensure_rows(idx_t desired_row)
{
    if (desired_row < rows_count) return;   /* nothing to do */

    /* Allocate / reallocate the array of row pointers. */
    rows = (seq_t **)realloc(rows,
                             (desired_row + 1) * sizeof(seq_t *));
    /* Construct each new row. */
    for (idx_t r = rows_count; r <= desired_row; ++r)
    {
        size_t len = ((size_t)1 << r);           /* 2^r elements */
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
        if (!curr) abort();                      /* out of memory */

        if (r == 0)                            /* base row */
        {
            curr[0] = (seq_t)1;
        }
        else
        {
            seq_t *prev      = rows[r - 1];
            size_t prev_len  = ((size_t)1 << (r - 1));
            seq_t   sum      = (seq_t)0;

            /* First half: cumulative sums of the previous row. */
            for (size_t i = 0; i < prev_len; ++i)
            {
                sum += prev[i];
                curr[i] = sum;
            }

            /* Second half: mirror of the first half. */
            for (size_t i = 0; i < prev_len; ++i)
                curr[len - 1 - i] = curr[i];
        }
        rows[r] = curr;
    }
    rows_count = desired_row + 1;
}

/* Public sequence function required by the harness */
seq_t a(idx_t n)
{
    /* Determine the row containing index n. */
    idx_t r = floor_log2_u64(n + 1);          /* 2^r <= n+1 < 2^(r+1) */
    ensure_rows(r);

    /* Offset of the first element of this row. */
    idx_t offset = ((idx_t)1 << r) - 1;
    idx_t pos    = n - offset;                /* position inside the row */
    return rows[r][pos];
}
