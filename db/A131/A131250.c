/*
 * A131250  A007318 * A004070.
 * https://oeis.org/A131250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A131250: Flattened triangle T(n,k) defined by
 *  T(0,0)=1,  T(n,0)=2^n,
 *  T(n,k)=2*T(n-1,k)+T(n-1,k-1)-T(n-2,k)   (1 ≤ k ≤ n)
 *  a(n) = T(r, k) where r = floor((sqrt(8n+1)-1)/2) and k = n – r(r+1)/2.
 *
 *  The implementation builds the triangle row‑by‑row using two previous rows
 *  and stores all values consecutively.  Each call computes all required
 *  rows up to the requested n and returns the cached value.
 */

#include <stdlib.h>

/* external types ------------------------------------------------------------- */
extern seq_t a(idx_t n);

/* implementation -------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* static cache of triangle values, in row‑major order */
    static seq_t *cache      = NULL;   /* flattened triangle */
    static size_t cache_len = 0;       /* number of cached elements */

    /* static rows: previous (n‑1) and two‑back (n‑2) */
    static seq_t *row_prev  = NULL;   /* row n‑1   */
    static size_t len_prev  = 0;       /* length of row_prev */
    static seq_t *row_prev2 = NULL;   /* row n‑2   */
    static size_t len_prev2 = 0;       /* length of row_prev2 */

    /* generate rows until cache contains index n */
    while (cache_len <= (size_t)n) {
        /* compute next row length (row index = len_prev) */
        size_t cur_len = len_prev + 1;   /* since len_prev == last row index +1 */

        /* allocate memory for the current row */
        seq_t *cur_row = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!cur_row) abort();         /* allocation failure */

        /* first element: T(row,0) = 2^row */
        if (len_prev == 0) {           /* row 0 */
            cur_row[0] = (seq_t)1;
        } else {
            /* double the previous first element */
            cur_row[0] = row_prev[0] + row_prev[0];
        }

        /* compute remaining elements of the row */
        for (size_t k = 1; k < cur_len; ++k) {
            seq_t t_prev  = (k < len_prev) ? row_prev[k]  : (seq_t)0;
            seq_t t_prev1 = row_prev[k - 1];
            seq_t t_prev2 = (k < len_prev2) ? row_prev2[k] : (seq_t)0;

            /* T(n,k) = 2*prev[k] + prev[k-1] - prev2[k] */
            seq_t tmp = t_prev + t_prev;    /* 2*prev[k] */
            tmp += t_prev1;
            tmp -= t_prev2;
            cur_row[k] = tmp;
        }

        /* append current row to cache */
        size_t new_len = cache_len + cur_len;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();       /* allocation failure */
        cache = new_cache;
        for (size_t i = 0; i < cur_len; ++i) {
            cache[cache_len + i] = cur_row[i];
        }
        cache_len = new_len;

        /* update row pointers for next iteration */
        free(row_prev2);
        row_prev2 = row_prev;
        len_prev2 = len_prev;

        row_prev = cur_row;
        len_prev = cur_len;
    }

    /* the requested element is now in the cache */
    return cache[(size_t)n];
}
