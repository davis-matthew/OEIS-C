/*
 * A059578  Variation of Boustrophedon transform applied to 1,1,1,1,... Fill
 *          an array by diagonals, all in the 'up' direction. The first column
 *          is 1,1,1,1,.... For the next element of a diagonal, add to the
 *          previous element the elements of the row and the column the new
 *          element is in. The first row gives a(n).
 * https://oeis.org/A059578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A059578: Variation of the Boustrophedon transform applied to
   the constant sequence 1,1,1,1,....  The array is filled by
   diagonals in the upward direction (from bottom‑left to
   top‑right).  The first column consists entirely of 1.  For a cell
   (i,j) with j>1 the value equals the previous cell on the same
   diagonal plus the sum of all already written values in row i and
   in column j.  The first row values A[1][n] give the sequence
   a(n). */

#include <stdlib.h>
#include <string.h>

/* static storage for computed values */
static seq_t *A      = NULL;   /* first‑row entries, 1‑based */
static seq_t *rowSum = NULL;   /* cumulative sums of each row, 1‑based */
static seq_t *colSum = NULL;   /* cumulative sums of each column, 1‑based */
static idx_t  max_n  = 0;      /* highest n that has been fully computed */

/* allocate or resize the three arrays so they can hold indices 0…N */
static void allocate_up_to(idx_t N)
{
    if (N <= max_n)
        return;

    /* new arrays, zero‑initialized */
    seq_t *newA     = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *newRow   = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *newCol   = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));

    /* copy old data if any */
    if (max_n > 0) {
        memcpy(newA + 1,     A   + 1, (size_t)max_n * sizeof(seq_t));
        memcpy(newRow + 1,   rowSum + 1, (size_t)max_n * sizeof(seq_t));
        memcpy(newCol + 1,   colSum + 1, (size_t)max_n * sizeof(seq_t));
    }

    /* free old memory (NULL is safe) */
    free(A);
    free(rowSum);
    free(colSum);

    A      = newA;
    rowSum = newRow;
    colSum = newCol;
    max_n  = N;
}

/* compute all diagonals up to the needed level so that a(n) for all
   n ≤ N are available.  This function is idempotent. */
static void ensure_up_to(idx_t N)
{
    if (N <= max_n)
        return;

    idx_t old_max = max_n;       /* remember the limit before growth */
    allocate_up_to(N);

    /* diagonals are numbered by d = i + j - 1.
       The first element on the d‑th diagonal is (i = min(d-1,N),
       j = d - i).  We need to process diagonals with d from
       old_max+2 up to N+1 (inclusive). */
    for (idx_t d = old_max + 2; d <= N + 1; ++d) {
        /* i starts at the largest possible row index on this
           diagonal and decreases to the smallest. */
        idx_t i_start = (d - 1 <= N) ? (d - 1) : N;
        idx_t i_end   = (d <= N)      ? 1       : (d - N);

        seq_t prev = 0;                 /* previous value on the diagonal */
        for (idx_t i = i_start; ; --i) {
            idx_t j = d - i;            /* column index */

            seq_t val;
            if (j == 1)
                val = 1;               /* the first column is all 1s */
            else
                val = prev + rowSum[i] + colSum[j];

            rowSum[i] += val;           /* update running sums */
            colSum[j] += val;

            if (i == 1)                /* the first row value of this column */
                A[j] = val;

            prev = val;

            if (i == i_end)
                break;
        }
    }
}

/* ------------------------------------------------------------------ */
/* public function required by the framework                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; behavior below is irrelevant */
        return 0;

    ensure_up_to(n);
    return A[n];
}
