/*
 * A164851  Generalized Lucas-Pascal triangle; (11*10^n, 1).
 * https://oeis.org/A164851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) from the Generalized Lucas‑Pascal triangle read
 *            row‑wise where T(r,k) is defined by T(0,0) = 1, T(n+1,0) =
 *            11*10^n, T(n,n) = 1, T(n,k) = T(n-1,k-1) + T(n-1,k) for 0 < k <
 *            n
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 54, u64 n <= 209, u128 n <= 779,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>       /* for malloc / realloc */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* static cache holding all previously computed values */
    static seq_t *cache = NULL;       /* array of a(i) for i < cache_cap */
    static size_t cache_cap = 0;      /* number of elements currently stored */
    static size_t rows_done = 0;      /* number of triangle rows already generated */

    /* initialise cache with the first row (row 0) */
    if (rows_done == 0) {
        cache      = (seq_t *)malloc(sizeof(seq_t));
        cache[0]  = 1;          /* T(0,0) */
        cache_cap = 1;
        rows_done = 1;
    }

    /* extend cache until it contains all indices up to n */
    while (cache_cap <= (size_t)n) {
        /* generate next row of the triangle */
        size_t r_cur        = rows_done;                     /* new row index, 0‑based */
        size_t prev_start   = (r_cur - 1) * r_cur / 2;       /* start index of row r_cur‑1 */
        size_t start_current= r_cur * (r_cur + 1) / 2;        /* start index of row r_cur   */
        size_t new_cap      = start_current + r_cur + 1;      /* capacity after adding row */

        /* reallocate cache to accommodate the new row */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – signal error by returning 0 (undefined for seq_t) */
            return 0;
        }
        cache = tmp;

        /* first element of the new row */
        if (r_cur == 0) {
            cache[start_current] = 1;                       /* should not occur in the loop */
        } else {
            seq_t val = pow10((idx_t)(r_cur - 1));
            val = val * 11;                                 /* 11 * 10^(r_cur-1) */
            cache[start_current] = val;
        }

        /* middle elements – combination of two elements from previous row */
        for (size_t i = 1; i < r_cur; ++i) {
            cache[start_current + i] =
                cache[prev_start + i - 1] + cache[prev_start + i];
        }

        /* last element of the row is always 1 */
        cache[start_current + r_cur] = 1;

        /* update bookkeeping */
        cache_cap = new_cap;
        rows_done = r_cur + 1;
    }

    /* return the requested term */
    return cache[(size_t)n];
}
