/*
 * A049324  A convolution triangle of numbers generalizing Pascal's triangle
 *          A007318.
 * https://oeis.org/A049324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 60, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/*  a(n,m) = 3*(3*m-n+1)*a(n-1,m)/n + m*a(n-1,m-1)/n   for n>=m>=1
    a(n,m)=0 if n<m or m=0;  a(1,1)=1
    a(n) is the flattened triangle row by row (ae.n mission: a(1)=1, a(2)=3, a(3)=1, ...).  */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;
    static size_t  cache_cap  = 0;

    /* previous row data to compute the next row */
    static seq_t *prev      = NULL;
    static size_t prev_len = 0;          /* length of previous row (row number) */

    if (n < 1) return 0;                 /* offset is 1, but guards against bad input */

    /* ensure cache has space for at least n terms */
    while (cache_size < n) {
        /* compute next row r = prev_len + 1 (row numbers are 1‑based) */
        idx_t r   = (idx_t)(prev_len + 1);
        size_t cur_len = (size_t)r;
        seq_t *cur = (seq_t*)malloc(cur_len * sizeof(seq_t));
        if (!cur) abort(); /* out of memory */

        /* make sure cache has enough space */
        if (cache_size + cur_len > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < cache_size + cur_len) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute the entries of the new row */
        for (size_t mi = 1; mi <= cur_len; ++mi) {
            seq_t val = 0;

            if (r == 1) {            /* base case a(1,1)=1 */
                val = 1;
            } else {
                /* first term: 3*(3*m-n+1)*a(n-1,m)/n  */
                int64_t factor = 3LL * ((int64_t)3 * (int64_t)mi - (int64_t)r + 1);
                if (factor > 0 && prev_len >= mi) {
                    val += (seq_t)factor * prev[mi - 1];
                }

                /* second term: m*a(n-1,m-1)/n  */
                if (mi >= 2 && prev_len >= mi - 1) {
                    val += (seq_t)mi * prev[mi - 2];
                }

                /* division by n is exact */
                val /= r;
            }

            cur[mi - 1] = val;
            cache[cache_size++] = val;
        }

        /* prepare for the next iteration */
        free(prev);
        prev = cur;
        prev_len = cur_len;
    }

    return cache[n - 1];
}
