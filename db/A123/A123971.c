/*
 * A123971  Triangle T(n,k), read by rows, defined by
 *          T(n,k)=3*T(n-1,k)-T(n-1,k-1)-T(n-2,k), T(0,0)=1, T(1,0)=2,
 *          T(1,1)=-1, T(n,k)=0 if k<0 or if k>n.
 * https://oeis.org/A123971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the flattening of the triangle T(n,k) read by rows,
 *            where T(n,k)=3*T(n-1,k)-T(n-1,k-1)-T(n-2,k), with base values
 *            T(0,0)=1, T(1,0)=2, T(1,1)=-1, and T(n,k)=0 if k<0 or k>n
 * Range:     i8 n <= 15, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    /* storage of the last two rows of the triangle */
    static seq_t *prev2 = NULL;   /* row r-2 */
    static seq_t *prev1 = NULL;   /* row r-1 */
    static idx_t next_row = 0;    /* next row to be generated */

    /* generate rows until the cache covers index n */
    while (cache_size <= n)
    {
        idx_t r = next_row;                      /* current row index          */
        size_t len = (size_t)r + 1;               /* number of elements in row  */
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
        if (!curr) return 0;                     /* allocation failure        */

        /* compute the current row */
        if (r == 0) {
            curr[0] = 1;
        } else if (r == 1) {
            curr[0] = 2;
            curr[1] = -1;
        } else {
            for (size_t k = 0; k <= r; ++k) {
                seq_t t1 = (k <= r - 1 ? prev1[k] : (seq_t)0);          /* T(r-1,k) */
                seq_t t2 = (k > 0 ? prev1[k - 1] : (seq_t)0);          /* T(r-1,k-1) */
                seq_t t3 = (k <= r - 2 ? prev2[k] : (seq_t)0);          /* T(r-2,k) */
                curr[k] = 3 * t1 - t2 - t3;
            }
        }

        /* ensure cache has room for the new elements */
        idx_t needed = cache_size + (idx_t)len;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? (size_t)cache_cap * 2 : 16;
            while (new_cap < (size_t)needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { free(curr); return 0; }
            cache = tmp;
            cache_cap = (idx_t)new_cap;
        }

        /* append new row to linear cache */
        for (size_t k = 0; k <= r; ++k)
            cache[cache_size++] = curr[k];

        /* shift row buffers for next iteration */
        if (prev2) free(prev2);
        prev2 = prev1;
        prev1 = curr;
        ++next_row;
    }

    return cache[n];
}
