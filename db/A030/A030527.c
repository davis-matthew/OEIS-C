/*
 * A030527  A convolution triangle of numbers obtained from A036083.
 * https://oeis.org/A030527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n,m) = (5*(5*m + n - 1)*a(n-1, m) + m*a(n-1, m-1)) / n, with
 *            a(1,1)=1. Flattened a(k) is the triangular reindex of a(n,m)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#define HAVE_U128 1
#else
#define HAVE_U128 0
#endif

seq_t a(idx_t n)
{
    static size_t cache_len = 0;          /* number of stored values            */
    static size_t cache_cap = 0;          /* allocated capacity for cache       */
    static seq_t *cache = NULL;           /* flattened triangle values          */
    static idx_t current_row = 0;         /* last fully computed row index      */
    static seq_t *prev_row = NULL;        /* array holding previous row values  */

    if (n < 1)
        return (seq_t)0;                  /* undefined for n < 1 */

    /* Compute rows until the cache has at least n elements. */
    while (cache_len < (size_t)n) {
        /* Next row index. */
        idx_t row = current_row + 1;
        size_t row_len = (size_t)row + 1; /* include dummy index 0 */

        /* Allocate space for the new row. */
        seq_t *curr = (seq_t *)malloc(row_len * sizeof(seq_t));
        if (!curr)
            return (seq_t)0;              /* allocation failure */

        curr[0] = (seq_t)0;                /* unused, serves as 0 for m=1 case */

        if (row == 1) {
            curr[1] = (seq_t)1;            /* base value a(1,1) */
        } else {
            for (idx_t m = 1; m <= row; ++m) {
                seq_t prev_m  = (m <= current_row) ? prev_row[m] : (seq_t)0;
                seq_t prev_m1 = ((m - 1) <= current_row) ? prev_row[m - 1] : (seq_t)0;
                seq_t val;

#if HAVE_U128
                u128 factor = (u128)5 * (((u128)5 * (u128)m + (u128)row - 1));
                u128 num = factor * (u128)prev_m + (u128)m * (u128)prev_m1;
                val = (seq_t)(num / (u128)row);
#else
                seq_t factor = (seq_t)5 * (((seq_t)5 * (seq_t)m + (seq_t)row - 1));
                seq_t num1 = factor * prev_m;
                seq_t num2 = (seq_t)m * prev_m1;
                val = (num1 + num2) / (seq_t)row;
#endif
                curr[m] = val;
            }
        }

        /* Append the computed row to the flat cache. */
        for (idx_t m = 1; m <= row; ++m) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(curr);
                    return (seq_t)0;      /* allocation failure */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = curr[m];
        }

        /* Prepare for next iteration. */
        free(prev_row);
        prev_row = curr;
        current_row = row;
    }

    return cache[(size_t)n - 1];
}
