/*
 * A049063  Triangle a(n,k) (0 <= k <= max(0, 2n-1)) of profile numbers.
 * https://oeis.org/A049063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabf
 * Formula:   triangle a(n,k) of profile numbers, flattened as a list.
 *            Recurrence (for k>0): a(n+1,k+1) = a(n,k) + 2*a(n,k-1) Initial
 *            values: a(n,0) = 1 for all n≥0 a(1,1) = 1 a(n,1) = 2 for n>1 The
 *            sequence a(n) is the flattened row‑major traversal of this
 *            triangle. The code builds rows on demand and caches t...
 * Range:     u8 n <= 51, u16 n <= 149, u32 n <= 496, u64 n <= 1872,
 *            u128 n <= 6942, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
# include <new>
# include <algorithm>
#endif

/* static cache of flattened triangle */
static seq_t *cache       = NULL;
static size_t cache_cap   = 0;   /* allocated capacity */
static size_t cache_used  = 0;   /* number of elements stored */

/* previous row used to generate the next one */
static seq_t *prev_row    = NULL;
static size_t prev_len    = 0;   /* length of prev_row   */
static idx_t  curr_row    = 0;   /* next row to generate */

/* ensure that rows 0 … up_to (inclusive) are computed and stored in cache */
static void ensure_rows(idx_t up_to)
{
    for (idx_t r = curr_row; r <= up_to; ++r)
    {
        /* length of the current row */
        size_t len_cur = (r == 0) ? 1 : 2 * (size_t)r;
        seq_t *cur = NULL;

#ifdef __cplusplus
        cur = new seq_t[len_cur];
#else
        cur = (seq_t*)malloc(len_cur * sizeof(seq_t));
#endif

        /* compute the entries of the current row */
        if (r == 0)
        {
            cur[0] = (seq_t)1;
        }
        else
        {
            cur[0] = (seq_t)1;
            cur[1] = (r == 1) ? (seq_t)1 : (seq_t)2;

            for (size_t k = 2; k < len_cur; ++k)
            {
                seq_t val = (seq_t)0;
                if (k - 1 < prev_len) val += prev_row[k - 1];
                if (k - 2 < prev_len) val += prev_row[k - 2] * (seq_t)2;
                cur[k] = val;
            }
        }

        /* grow the cache if necessary */
        if (cache_used + len_cur > cache_cap)
        {
            size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
            while (new_cap < cache_used + len_cur) new_cap <<= 1;

#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_cap];
            if (cache)
                std::copy(cache, cache + cache_used, tmp);
            delete[] cache;
            cache = tmp;
#else
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
#endif
            cache_cap = new_cap;
        }

        /* copy the current row into the cache */
        for (size_t i = 0; i < len_cur; ++i)
            cache[cache_used++] = cur[i];

        /* free the previous row */
        if (prev_row)
        {
#ifdef __cplusplus
            delete[] prev_row;
#else
            free(prev_row);
#endif
        }

        prev_row = cur;
        prev_len = len_cur;
        ++curr_row;
    }
}

seq_t a(idx_t n)
{
    /* fast path for already cached indices */
    if (n < cache_used) return cache[(size_t)n];

    /* find the row that contains index n:
       the r‑th row (0‑based) holds indices in
       [ (r‑1)²+(r‑1)+1 , r²+r+1 )   for r>0
       equivalently the smallest r with r²+r+1 > n   */
    idx_t r = 0;
    while ((seq_t)r * (seq_t)(r + 1) + (seq_t)1 <= n) ++r;

    /* generate rows up to r and return the requested element */
    ensure_rows(r);
    return cache[(size_t)n];
}
