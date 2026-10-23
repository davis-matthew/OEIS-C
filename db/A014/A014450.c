/*
 * A014450  Even numbers in the triangle of Eulerian numbers.
 * https://oeis.org/A014450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look, easy
 * Formula:   a(n) = the n-th even Eulerian number in the Eulerian triangle,
 *            read row‑by‑row left‑to‑right
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 44, u64 n <= 132, u128 n <= 358,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Allocate an array of seq_t that works for both C (malloc) and C++ (new[]).
   The corresponding free_array handles the appropriate deallocation. */
static seq_t *alloc_array(size_t len)
{
#if defined(__cplusplus)
    return new seq_t[len];
#else
    return (seq_t *)malloc(len * sizeof(seq_t));
#endif
}

static void free_array(seq_t *p)
{
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    idx_t target = n;          /* desired index (1‑based) */
    idx_t even_cnt = 0;        /* number of even Eulerian numbers seen */

    /* prev holds the previous row.  prev_len is the number of elements. */
    seq_t *prev = NULL;
    idx_t prev_len = 0;

    /* Enumerate rows of the Eulerian triangle: row 1,2,3,... */
    for (idx_t row = 1; ; ++row)
    {
        seq_t *cur = alloc_array(row);
        if (!cur) {             /* allocation failed – unlikely but safe */
            if (prev) free_array(prev);
            return (seq_t)0;
        }

        /* Compute the current row. */
        if (row == 1)
        {
            cur[0] = (seq_t)1;
        }
        else
        {
            for (idx_t k = 0; k < row; ++k)
            {
                seq_t part1 = 0;   /* A(row-1, k)   (0 when k == prev_len) */
                seq_t part2 = 0;   /* A(row-1, k-1) (0 when k == 0)          */

                if (k < prev_len) part1 = prev[k];
                if (k > 0)        part2 = prev[k - 1];

                cur[k] = (seq_t)(k + 1) * part1 + (seq_t)(row - k) * part2;
            }
        }

        /* Scan the row for even entries. */
        for (idx_t k = 0; k < row; ++k)
        {
            if ((cur[k] & (seq_t)1) == (seq_t)0)   /* even? */
            {
                ++even_cnt;
                if (even_cnt == target)
                {
                    seq_t result = cur[k];
                    if (prev) free_array(prev);
                    free_array(cur);
                    return result;
                }
            }
        }

        /* Prepare for next iteration.  prev becomes cur. */
        if (prev) free_array(prev);
        prev     = cur;
        prev_len = row;
    }
}
