/*
 * A202395  Triangle T(n,k), read by rows, given by (1, 1, 1, 0, 0, 0, 0, 0,
 *          0, 0, ...) DELTA (1, 1, -1, 0, 0, 0, 0, 0, 0, 0, ...) where DELTA
 *          is the operator defined in A084938.
 * https://oeis.org/A202395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,0)+T(n,1)+…+T(n,n) where the triangular array T(n,k)
 *            obeys T(n,k) = 3*T(n-1,k) + T(n-1,k-1) + T(n-2,k-2) - T(n-2,k)
 *            with T(0,0)=T(1,0)=T(1,1)=1 and T(n,k)=0 for k<0 or k>n
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state to cache the sequence values and the
       last two rows of the triangle, so that calls in any
       order can be answered efficiently. */
    static seq_t *cache  = NULL;   /* cache[n] = a(n) */
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* capacity of the cache buffer */

    static seq_t *row  = NULL;     /* current row T(n-1,*)   */
    static size_t row_len  = 0;    /* length of current row   */
    static seq_t *prev = NULL;     /* previous row T(n-2,*)   */
    static size_t prev_len = 0;    /* length of previous row  */

    static int initialized = 0;    /* first‑time initialisation flag */

    /* ----------------- initialise ----------------- */
    if (!initialized) {
        cache_cap   = 64;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]    = (seq_t)1;            /* a(0) = T(0,0) */
        cache_len   = 1;

        row_len = 1;
        row     = (seq_t *)malloc(row_len * sizeof(seq_t));
        row[0]  = (seq_t)1;                /* row 0 = {T(0,0)} */

        prev_len = 0;
        prev     = NULL;

        initialized = 1;
    }

    /* ------------------------------------------------ */
    /* Generate rows until the cache contains index n */
    while (cache_len <= (size_t)n) {
        size_t new_len = row_len + 1;            /* length of the next row */
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));

        /* Special case: first transition from row 0 to row 1 */
        if (row_len == 1 && prev_len == 0) {
            new_row[0] = (seq_t)1;
            new_row[1] = (seq_t)1;
        } else {
            /* General recurrence using the two previous rows */
            for (size_t k = 0; k < new_len; ++k) {
                seq_t val = (seq_t)0;

                if (k < row_len)
                    val += (seq_t)3 * row[k];        /* 3*T(n-1,k) */

                if (k > 0)
                    val += row[k - 1];               /* +T(n-1,k-1) */

                if (prev_len > 0 && k >= 2 && k - 2 < prev_len)
                    val += prev[k - 2];             /* +T(n-2,k-2) */

                if (prev_len > 0 && k < prev_len)
                    val -= prev[k];                 /* -T(n-2,k) */

                new_row[k] = val;
            }
        }

        /* Append the new row to the cache */
        size_t needed = cache_len + new_len;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while (new_cap < needed) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        for (size_t i = 0; i < new_len; ++i)
            cache[cache_len++] = new_row[i];

        /* Shift rows: prev <- row, row <- new_row */
        seq_t *old_prev  = prev;
        prev            = row;
        prev_len        = row_len;

        row     = new_row;
        row_len = new_len;

        free(old_prev);   /* old prev is no longer needed */
    }

    return cache[(size_t)n];
}
