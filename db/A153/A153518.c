/*
 * A153518  Triangular T(n,k) = T(n-1, k) + T(n-1, k-1) + 5*T(n-2, k-1), read
 *          by rows.
 * https://oeis.org/A153518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   T(1,1) = 2 T(2,1) = T(2,2) = 5 T(3,1) = T(3,3) = 2, T(3,2) = 46
 *            T(4,1) = T(4,4) = 2, T(4,2) = T(4,3) = 123 For n ≥ 5, 1 ≤ k ≤ n:
 *            T(n,k) = T(n‑1,k) + T(n‑1,k‑1) + 5*T(n‑2,k‑1), where
 *            out‑of‑range indices are treated as 0. The sequence a(n) is
 *            obtained by reading the rows of T(n,k) left to right (a...
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 179, u64 n <= 683,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **rows = NULL;
static size_t rows_allocated = 0;
static size_t rows_used     = 0;

/* Build rows up to the given number (inclusive). */
static void ensure_rows(idx_t up_to)
{
    if (rows_used >= (size_t)up_to)
        return;

    /* enlarge the rows array if needed */
    if (rows_allocated < (size_t)up_to)
    {
        size_t new_cap = rows_allocated ? rows_allocated * 2 : 4;
        while (new_cap < (size_t)up_to)
            new_cap *= 2;
        rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        rows_allocated = new_cap;
    }

    /* compute rows */
    for (size_t n = rows_used + 1; n <= (size_t)up_to; ++n)
    {
        seq_t *row = (seq_t*)malloc(n * sizeof(seq_t));
        if (!row) abort();        /* memory allocation failure */

        if (n == 1)
        {
            row[0] = (seq_t)2;
        }
        else if (n == 2)
        {
            row[0] = (seq_t)5;
            row[1] = (seq_t)5;
        }
        else if (n == 3)
        {
            row[0] = (seq_t)2;
            row[1] = (seq_t)46;
            row[2] = (seq_t)2;
        }
        else if (n == 4)
        {
            row[0] = (seq_t)2;
            row[1] = (seq_t)123;
            row[2] = (seq_t)123;
            row[3] = (seq_t)2;
        }
        else /* n >= 5 */
        {
            size_t prev  = n - 1;
            size_t prev2 = n - 2;
            for (size_t k = 0; k < n; ++k)
            {
                seq_t a = (k < prev) ? rows[prev][k] : 0;
                seq_t b = (k >= 1)   ? rows[prev][k-1] : 0;
                seq_t c = 0;
                if (k >= 1 && (k-1) < prev2)
                    c = rows[prev2][k-1] * (seq_t)5;
                row[k] = a + b + c;
            }
        }

        rows[n] = row;
    }

    rows_used = up_to;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset 1; callers should not ask for n=0 */

    /* Find the row that contains the n‑th term. */
    idx_t row = 0;
    idx_t cumulative = 0;
    while (cumulative + row + 1 < n)
    {
        ++row;
        cumulative += row;
    }
    ++row;   /* now 'row' is 1‑based row number */

    /* Ensure that rows up to this number are computed. */
    ensure_rows(row);

    /* Compute column position within the row (1‑based). */
    idx_t col = n - cumulative;   /* 1‑based column */

    return rows[row][col - 1];
}
