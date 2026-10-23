/*
 * A119865  Triangle T(n,k), 0<=k<=n, read by rows, given by [1, 1, 0, 1, 0,
 *          0, 0, 0, 0, ...] DELTA [1, 0, 1, 0, 1, 0, 0, 0, 0, 0, ...] where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A119865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A119865 – Triangle T(n,k)
   Recurrence:
       T(n,k) = 3·T(n‑1,k) + 3·T(n‑1,k‑1)
                – 2·T(n‑2,k) – 4·T(n‑2,k‑1) – 3·T(n‑2,k‑2)
                +   T(n‑3,k‑2) +   T(n‑3,k‑3)
   with base rows:
       T(0,0) = 1
       T(1,0) = 1 , T(1,1) = 1
       T(2,0) = 2 , T(2,1) = 3 , T(2,2) = 1
   The sequence a(n) is the triangle read by rows, starting with
   a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=1, …
*/

#include <stdlib.h>

/* Storage for computed rows – each row is an array of seq_t of length (n+1) */
static seq_t **rows = NULL; /* array of pointers to rows */
static idx_t row_count = 0; /* number of rows currently stored */
static size_t row_cap   = 0;/* capacity of the rows pointer array */

/* Ensure that rows[0..r] are computed and stored. */
static void ensure_rows(idx_t r)
{
    if (row_count > r) return;

    /* Grow the rows pointer array if needed */
    if (row_cap < (size_t)(r + 1))
    {
        size_t new_cap = row_cap ? row_cap * 2 : 1;
        while (new_cap <= (size_t)(r + 1)) new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) abort(); /* allocation failed */
        rows = tmp;
        row_cap = new_cap;
    }

    /* Allocate and compute new rows from row_count to r */
    for (idx_t i = row_count; i <= r; ++i)
    {
        seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (!row) abort(); /* allocation failed */
        rows[i] = row;

        if (i == 0)
        {
            row[0] = (seq_t)1;
        }
        else if (i == 1)
        {
            row[0] = (seq_t)1;
            row[1] = (seq_t)1;
        }
        else if (i == 2)
        {
            row[0] = (seq_t)2;
            row[1] = (seq_t)3;
            row[2] = (seq_t)1;
        }
        else /* use recurrence for i >= 3 */
        {
            for (idx_t k = 0; k <= i; ++k)
            {
                seq_t pos = 0;
                seq_t neg = 0;

                /* +3*T(i-1,k) */
                if (k <= i - 1) pos += (seq_t)3 * rows[i - 1][(size_t)k];
                /* +3*T(i-1,k-1) */
                if (k >= 1 && k - 1 <= i - 1)
                    pos += (seq_t)3 * rows[i - 1][(size_t)(k - 1)];
                /* +T(i-3,k-2) */
                if (i >= 3 && k >= 2 && k - 2 <= i - 3)
                    pos += rows[i - 3][(size_t)(k - 2)];
                /* +T(i-3,k-3) */
                if (i >= 3 && k >= 3 && k - 3 <= i - 3)
                    pos += rows[i - 3][(size_t)(k - 3)];

                /* -2*T(i-2,k) */
                if (k <= i - 2) neg += (seq_t)2 * rows[i - 2][(size_t)k];
                /* -4*T(i-2,k-1) */
                if (k >= 1 && k - 1 <= i - 2)
                    neg += (seq_t)4 * rows[i - 2][(size_t)(k - 1)];
                /* -3*T(i-2,k-2) */
                if (k >= 2 && k - 2 <= i - 2)
                    neg += (seq_t)3 * rows[i - 2][(size_t)(k - 2)];

                row[(size_t)k] = pos - neg;
            }
        }
    }

    row_count = r + 1;
}

/* Return the n-th term of the sequence (a(0)=1, a(1)=1, …). */
seq_t a(idx_t n)
{
    /* Find the row that contains the n-th term.
       The first term of row r starts at index r*(r+1)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    ensure_rows(r);

    /* offset of n within row r */
    seq_t before = (seq_t)r * ((seq_t)r + 1) / 2;
    idx_t pos = (idx_t)((seq_t)n - before);

    return rows[(size_t)r][(size_t)pos];
}
