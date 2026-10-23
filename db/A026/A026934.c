/*
 * A026934  a(n) = Sum_{k=0..n-1} T(n,k) * T(n,k+1), with T given by A008288.
 * https://oeis.org/A026934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} T(n,k) * T(n,k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where T(n,k) is defined by  T(0,0)=1,  */
/* T(n,k)=T(n-1,k-1)+T(n-1,k)+T(n-1,k+1)  with 0-based indices. */

#include <stdlib.h>

#ifdef __cplusplus
#  define ALLOC_SEQ(n)    new seq_t[(n)]
#  define FREE_SEQ(p)     delete [] (p)
#else
#  define ALLOC_SEQ(n)    ((seq_t *)malloc((n)*sizeof(seq_t)))
#  define FREE_SEQ(p)     free((void*)(p))
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* offset is 1, but guard against n==0 */

    /* Persistent state for memoisation */
    static seq_t *prev_prev_row = NULL;
    static seq_t *prev_row     = NULL;
    static size_t prev_prev_size = 0;
    static size_t prev_size     = 0;

    static seq_t *cache = NULL;
    static size_t cache_cap   = 0;
    static idx_t  max_precomputed = 0;

    /* ---------- initialise for first call ---------- */
    if (max_precomputed == 0)
    {
        /* Row 0 : T(0,0)=1 */
        prev_prev_row = ALLOC_SEQ(1);
        if (!prev_prev_row) return (seq_t)0;
        prev_prev_row[0] = (seq_t)1;
        prev_prev_size = 1;

        /* Row 1 : T(1,0)=T(1,1)=1 */
        prev_row = ALLOC_SEQ(2);
        if (!prev_row) { FREE_SEQ(prev_prev_row); return (seq_t)0; }
        prev_row[0] = prev_row[1] = (seq_t)1;
        prev_size = 2;

        /* Allocate cache for a(1) */
        cache = ALLOC_SEQ(1);
        if (!cache) { FREE_SEQ(prev_prev_row); FREE_SEQ(prev_row); return (seq_t)0; }
        cache_cap = 1;

        /* a(1) = T(1,0)*T(1,1) = 1 */
        cache[0] = (seq_t)1;
        max_precomputed = 1;
    }

    /* --------- compute up to requested n if needed ---------- */
    while (max_precomputed < n)
    {
        idx_t   m = max_precomputed + 1;           /* next row index to compute */
        size_t  new_size = (size_t)m + 1;          /* number of elements in new row */

        /* Allocate new row T(m, *) */
        seq_t *new_row = ALLOC_SEQ(new_size);
        if (!new_row) return (seq_t)0;

        /* Compute T(m,k) for k=0..m */
        new_row[0] = (seq_t)1;                     /* base case T(m,0)=1 */
        for (size_t k = 1; k < new_size; ++k)
        {
            seq_t term = (seq_t)0;

            if (k < prev_size)
                term += prev_row[k];

            if (k-1 < prev_size)
                term += prev_row[k-1];

            if (k-1 < prev_prev_size)
                term += prev_prev_row[k-1];

            new_row[k] = term;
        }

        /* Compute a(m) = sum_{k=0}^{m-1} T(m,k)*T(m,k+1) */
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < new_size - 1; ++k)
            sum += new_row[k] * new_row[k + 1];

        /* Ensure cache capacity */
        if ((size_t)m > cache_cap)
        {
            size_t new_cap = (size_t)m;
            seq_t *tmp;
#ifdef __cplusplus
            tmp = new seq_t[new_cap];
            for (size_t i = 0; i < cache_cap; ++i)
                tmp[i] = cache[i];
            delete [] cache;
#else
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
            if (!tmp)
            {
                /* Allocation failed – clean up */
                FREE_SEQ(new_row);
                FREE_SEQ(prev_prev_row);
                FREE_SEQ(prev_row);
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[(size_t)m - 1] = sum;   /* store a(m) */

        /* Shift rows for next iteration */
        seq_t *old_prev_prev = prev_prev_row;
        prev_prev_row = prev_row;
        prev_prev_size = prev_size;

        prev_row = new_row;
        prev_size = new_size;

        FREE_SEQ(old_prev_prev);

        ++max_precomputed;
    }

    /* Return the requested value */
    return cache[(size_t)n - 1];
}
