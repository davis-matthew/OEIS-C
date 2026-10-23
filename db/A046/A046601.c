/*
 * A046601  First numerator and then denominator of the 1/5-Pascal triangle
 *          (by row). To get a 1/5-Pascal triangle, replace "2" in the third
 *          row of the Pascal triangle with "1/5" and calculate all other rows
 *          as in the Pascal triangle.
 * https://oeis.org/A046601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A046601 – flattened 1/5‑Pascal triangle.
   For each entry (r,k) of the triangle defined by
       S(0,0) = 5,
       S(1,0) = S(1,1) = 5,
       S(2,1) = 1,
       S(r,0) = S(r,r) = 5,
       S(r,k) = S(r-1,k-1) + S(r-1,k)               (1 ≤ k < r),
   we output the reduced numerator then denominator of the fraction
   S(r,k) / 5.  The terms a(n) are the numerators/denominators
   interleaved row by row.                     */

#include <stdlib.h>

/* Stored rows of the modified Pascal triangle ``S``.
   Each row r contains r+1 elements of type seq_t.  Rows are allocated
   lazily and kept for future calls.  */
static seq_t **S_rows      = NULL;
static idx_t   S_rows_used = 0;      /* number of rows currently stored */

/* Ensure that row r (0‑indexed) exists, computing any missing rows. */
static void ensure_rows(idx_t r)
{
    while (S_rows_used <= r) {
        /* enlarge the array of row pointers */
        seq_t **tmp = (seq_t **)realloc(S_rows,
                         (S_rows_used + 1) * sizeof(seq_t *));
        if (!tmp) abort();
        S_rows = tmp;

        /* allocate the new row */
        seq_t *row = (seq_t *)malloc((S_rows_used + 1) * sizeof(seq_t));
        if (!row) abort();

        if (S_rows_used == 0) {          /* row 0 */
            row[0] = (seq_t)5;
        } else if (S_rows_used == 1) {   /* row 1 */
            row[0] = (seq_t)5;
            row[1] = (seq_t)5;
        } else if (S_rows_used == 2) {   /* row 2 : [5,1,5] */
            row[0] = (seq_t)5;
            row[1] = (seq_t)1;
            row[2] = (seq_t)5;
        } else {                         /* row r ≥ 3 */
            row[0] = (seq_t)5;
            row[S_rows_used] = (seq_t)5;
            for (idx_t k = 1; k < S_rows_used; ++k) {
                row[k] = S_rows[S_rows_used - 1][k - 1] +
                          S_rows[S_rows_used - 1][k];
            }
        }

        S_rows[S_rows_used] = row;
        ++S_rows_used;
    }
}

/* Return the ath term of sequence A046601 (1‑indexed). */
seq_t a(idx_t n)
{
    /* 1. locate the row r such that (r+1)(r+2) ≥ n . */
    idx_t r = 0;
    while ((r + 1) * (r + 2) < n) ++r;

    /* 2. number of terms before row r : r*(r+1). */
    idx_t before = r * (r + 1);

    /* 3. zero‑based position t in row r (0 … 2*(r+1)-1). */
    idx_t t = n - before - 1ULL;   /* -1 to make zero‑based */

    /* 4. column k in the triangle and whether we need numerator or denominator. */
    idx_t k   = t / 2;          /* column (0‑based) */
    int    pos = t % 2;         /* 0 → numerator, 1 → denominator */

    /* 5. ensure the required rows have been computed. */
    ensure_rows(r);

    /* 6. value S(r,k). */
    seq_t val = S_rows[r][k];

    /* 7. reduce S/5 to simplest fraction. */
    if (pos == 0) { /* numerator */
        if (val % (seq_t)5 == 0)
            return val / (seq_t)5;   /* (S/5) / 1 */
        return val;                  /* numerator S */
    } else {                 /* denominator */
        if (val % (seq_t)5 == 0)
            return (seq_t)1;   /* denominator becomes 1 */
        return (seq_t)5;       /* denominator remains 5 */
    }
}
