/*
 * A014643  Triangular array starting with {1,1}; then i-th term in a row
 *          gives number of i's in next row.
 * https://oeis.org/A014643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 6807, u32 n <= 6807, u64 n <= 6807,
 *            u128 n <= 6807, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: start with the row {1,1}.  
   The i‑th term in a row gives the number of i's in the next row.  
   Thus the next row contains the number j repeated as many times as
   the j‑th term of the current row (j starting from 1).  
   The sequence a(n) is obtained by writing the rows one after another
   in row‑major order.  No closed formula is known.  The code builds
   the triangle lazily and caches all values for fast repeated queries.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for 0 – return 0 as a sentinel */
        return (seq_t)0;

    /* static cache for the flattened sequence */
    static seq_t *cache     = NULL;
    static idx_t  cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* allocated capacity          */

    /* static state for the last computed row */
    static seq_t *row_prev = NULL;
    static idx_t  row_len  = 0;

    /* initialise the first row and the cache if this is the first call */
    if (cache == NULL) {
        /* first row {1,1} */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;

        row_len = 2;
        row_prev = (seq_t *)malloc(row_len * sizeof(seq_t));
        if (row_prev == NULL) return (seq_t)0;
        row_prev[0] = (seq_t)1;
        row_prev[1] = (seq_t)1;
    }

    /* grow the triangle until we have at least n elements in the cache */
    while (cache_len < n) {
        /* compute the length of the next row: sum of counts in row_prev */
        seq_t sum_seq = (seq_t)0;
        for (idx_t i = 0; i < row_len; ++i)
            sum_seq += row_prev[i];
        idx_t next_len = (idx_t)sum_seq;            /* fits in idx_t for n ≤ 10000 */

        /* ensure the cache can hold the new elements */
        size_t needed = cache_len + next_len;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 1;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* allocate storage for the new row */
        seq_t *new_row = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (new_row == NULL) return (seq_t)0;

        /* produce the next row and append its values to the cache */
        idx_t pos = 0;                              /* position in new_row */
        for (idx_t i = 0; i < row_len; ++i) {
            idx_t count = (idx_t)row_prev[i];        /* how many i+1's in next row */
            seq_t value = (seq_t)(i + 1);
            for (idx_t k = 0; k < count; ++k) {
                cache[cache_len++] = value;         /* store in flattened cache */
                new_row[pos++] = value;             /* add to the new row */
            }
        }

        /* replace the old row with the new one */
        free(row_prev);
        row_prev = new_row;
        row_len  = next_len;
    }

    return cache[n - 1];    /* a(1) is cache[0] */
}
