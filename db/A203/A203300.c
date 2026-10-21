/*
 * A203300  Self-generating triangle based on symmetric functions.
 * https://oeis.org/A203300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 18, u32 n <= 23, u64 n <= 25, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203300 – Self‑generating triangle of elementary symmetric functions
   (OEIS A203300). Each row n (starting with 1) has n coefficients c₀ … c_{n-1}.
   For n>1 the coefficients are the elementary symmetric sums of the previous
   row (b₀ … b_{n-2}), i.e.
   ∏_{i=0}^{n-2} (x + b_i) = x^{n-1} + c₀ x^{n-2} + … + c_{n-2}.
   The sequence is read row by row; a(1) is the first term of row 1,
   a(2) the first term of row 2, a(3) the second term of row 2, and so on. */

#include <stdint.h>

#define MAX_ROWS 300   /* sufficient for n up to about 10 000 (row ~141) */

static seq_t triangle[MAX_ROWS][MAX_ROWS];
static idx_t computed_rows = 0;

/* Compute all rows up to r (inclusive).  The global array 'triangle'
   is filled with the elementary symmetric sums. */
static void compute_rows_upto(idx_t r)
{
    if (r <= computed_rows)
        return;

    for (idx_t i = computed_rows + 1; i <= r; ++i)
    {
        if (i == 1)
        {
            triangle[1][0] = 1;
        }
        else
        {
            idx_t m = i - 1;
            triangle[i][0] = 1;
            for (idx_t j = 1; j < i; ++j)
                triangle[i][j] = 0;      /* initialise remaining entries */

            /* Update elementary symmetric sums using the previous row */
            for (idx_t bi = 0; bi < m; ++bi)
            {
                seq_t b = triangle[i - 1][bi];
                for (idx_t j = m; j > 0; --j)
                {
                    triangle[i][j] = triangle[i][j] + triangle[i][j - 1] * b;
                }
            }
        }
    }
    computed_rows = r;
}

seq_t a(idx_t n)
{
    /* Find the row r such that 1 + 2 + … + r ≥ n */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n)
    {
        ++r;
        cum += r;
    }

    idx_t prev_cum = cum - r;          /* sum of lengths of rows < r */
    idx_t pos = n - prev_cum - 1;      /* 0‑based position in row r */

    compute_rows_upto(r);
    return triangle[r][pos];
}
