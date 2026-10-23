/*
 * A049224  A convolution triangle of numbers obtained from A025751.
 * https://oeis.org/A049224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n, m) = 6*(6*(n-1)-m)*a(n-1, m)/n + m*a(n-1, m-1)/n, for n ≥ m
 *            ≥ 1; a(1,1)=1, a(n,0)=0, a(n,m)=0 for n<m. The sequence a(n)
 *            enumerates the triangle row‑major order
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 28, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of all triangle entries in row‑major order */
    static seq_t *cache = NULL;        /* flat array of computed entries    */
    static size_t cache_len = 0;       /* number of stored entries          */
    static idx_t  max_row = 0;         /* last row that has been computed   */
    static seq_t *prev_row = NULL;     /* array holding previous row values  */
    static idx_t  prev_len = 0;        /* length of previous row            */

    /* compute rows until we have at least n entries */
    while (cache_len < (size_t)n)
    {
        idx_t row = max_row + 1;               /* new row to compute (1‑based)     */
        size_t len = (size_t)row;              /* number of elements in the row   */
        seq_t *row_vals = (seq_t *)malloc((len + 1) * sizeof(seq_t));
        if (!row_vals)          /* allocation failed */
            return 0;

        if (row == 1)
        {
            /* first row */
            row_vals[1] = 1;
        }
        else
        {
            /* compute all entries of the new row */
            for (size_t m = 1; m <= len; ++m)
            {
                if (m == len)
                {
                    /* a(n,n) = a(n-1,n-1) */
                    row_vals[m] = prev_row[m - 1];
                }
                else
                {
                    /* recurrence */
                    seq_t factor = 6 * ((seq_t)(6 * (row - 1) - m));
                    seq_t part1 = factor * prev_row[m];
                    seq_t part2 = 0;
                    if (m > 1)
                        part2 = (seq_t)m * prev_row[m - 1];
                    row_vals[m] = (part1 + part2) / row;
                }
            }
        }

        /* append the new row to the global cache */
        size_t new_cache_len = cache_len + len;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cache_len * sizeof(seq_t));
        if (!new_cache)
        {
            free(row_vals);
            return 0;
        }
        cache = new_cache;
        for (size_t m = 1; m <= len; ++m)
            cache[cache_len++] = row_vals[m];

        /* release the previous row to avoid leaks */
        if (prev_row) free(prev_row);
        prev_row = row_vals;
        prev_len = len;
        ++max_row;
    }

    return cache[(size_t)n - 1];
}
