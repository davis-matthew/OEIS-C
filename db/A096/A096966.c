/*
 * A096966  Triangle (read by rows) in which the number of entries in a row
 *          only increases by 1 every other row, the first column and the
 *          'diagonal' is set to all 1's and a(i,j) = a(i-1,j) + a(i-1,j-1) +
 *          a(i-2,j-1) + a(i-3,j-1) for other entries.
 * https://oeis.org/A096966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Triangle T with rows indexed by i≥0. Row i has length
 *            L(i)=i/2+1. The first element of every row is 1. If i is even,
 *            the last element of row i is also 1. For all other positions
 *            (i,j) (where 0<j<L(i)-1 or (i odd)): T(i,j) = T(i-1,j) +
 *            T(i-1,j-1) + T(i-2,j-1) + T(i-3,j-1) The sequence a(n) is
 *            obta...
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for the triangle flattened and the row offsets */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static idx_t *row_offset = NULL;
    static size_t row_offset_len = 0, row_offset_cap = 0;
    static idx_t next_row = 0;            /* index of the next row to compute */

    /* build rows until we have at least n+1 terms in the cache */
    while (cache_size <= n)
    {
        idx_t i = next_row;                       /* current row index */
        idx_t len_i = (i / 2) + 1;                /* length of row i */

        /* grow the cache if necessary */
        if (cache_size + (size_t)len_i > cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < cache_size + (size_t)len_i) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* grow the offset array if necessary */
        if (row_offset_len + 1 > row_offset_cap)
        {
            size_t new_cap = row_offset_cap ? row_offset_cap * 2 : 16;
            idx_t *tmp = (idx_t *)realloc(row_offset, new_cap * sizeof(idx_t));
            row_offset = tmp;
            row_offset_cap = new_cap;
        }

        /* offset of the current row in the flattened array */
        idx_t offset_i = (idx_t)cache_size;
        row_offset[row_offset_len++] = offset_i;

        /* compute each element of the current row */
        for (idx_t j = 0; j < len_i; ++j)
        {
            seq_t val;
            /* first column or even‑row diagonal are 1 */
            if (j == 0 || ((i % 2 == 0) && j == len_i - 1))
            {
                val = (seq_t)1;
            }
            else
            {
                val = (seq_t)0;
                /* a(i-1,j) */
                if (i >= 1)
                {
                    idx_t len_prev = ((i - 1) / 2) + 1;
                    if (j < len_prev)
                        val += cache[row_offset[row_offset_len - 2] + j];
                    if (j > 0 && (j - 1) < len_prev)
                        val += cache[row_offset[row_offset_len - 2] + (j - 1)];
                }
                /* a(i-2,j-1) */
                if (i >= 2)
                {
                    idx_t len_prev2 = ((i - 2) / 2) + 1;
                    if (j > 0 && (j - 1) < len_prev2)
                        val += cache[row_offset[row_offset_len - 3] + (j - 1)];
                }
                /* a(i-3,j-1) */
                if (i >= 3)
                {
                    idx_t len_prev3 = ((i - 3) / 2) + 1;
                    if (j > 0 && (j - 1) < len_prev3)
                        val += cache[row_offset[row_offset_len - 4] + (j - 1)];
                }
            }
            cache[cache_size++] = val;
        }

        ++next_row;   /* next row to compute */
    }

    return cache[n];
}
