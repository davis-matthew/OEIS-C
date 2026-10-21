/*
 * A099891  XOR difference triangle of A003188 (Gray code numbers), read by
 *          rows.
 * https://oeis.org/A099891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* XOR difference triangle of A003188 (Gray code numbers), read by rows.
   Sequence a(n) is the serial traversal of the triangle T where
   T(0,0)=gray(0)=0,
   T(r,0)=gray(r) and T(r,k)=T(r,k-1) ^ T(r-1,k-1) for k>0.
   Formula: a(n) = the (n+1)th entry in the row‑by‑row traversal of T. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* We cannot use malloc/realloc in C++ for a non‑trivial class type.
   Use operator new/new[] when compiled as C++, and fall back to
   malloc/free when compiled as plain C.  */
/* allocate array of seq_t */
static seq_t *alloc_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n]();          /* default‑construct elements      */
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}
/* free array of seq_t */
static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}
/* grow an array of seq_t, copying old contents                                       */
static void grow_seq_array(seq_t *cur, size_t old_cap, size_t new_cap, seq_t **out)
{
    seq_t *tmp = alloc_seq_array(new_cap);
    for (size_t i = 0; i < old_cap; ++i)
        tmp[i] = cur[i];
    free_seq_array(cur);
    *out = tmp;
}

/* cache of the sequence values */
static seq_t  *cache      = NULL;   /* array of computed values                     */
static size_t cache_cap   = 0;       /* allocated capacity of cache                  */
static size_t cache_len   = 0;       /* number of elements actually stored in cache */

/* previous row of the triangle (0‑indexed) */
static seq_t *prev_row     = NULL;   /* array of values of the previous row          */
static idx_t  prev_row_len = 0;      /* length of that row (row+1)                   */

/* the last row that has been completely generated */
static idx_t max_row = (idx_t)-1;     /* -1 = no row computed yet                       */

/* make sure cache contains at least indices 0 .. n inclusive               */
static void ensure_up_to(idx_t n)
{
    while (cache_len <= (size_t)n) {
        /* add row r = max_row + 1                                         */
        idx_t r = max_row + 1;
        size_t rsize = (size_t)(r + 1);             /* row length          */

        /* allocate current row                                             */
        seq_t *cur = alloc_seq_array(rsize);
        if (!cur) abort();

        /* first entry of the row is gray(r)                               */
        seq_t gray = ((seq_t)r) ^ (((seq_t)r) >> 1);
        cur[0] = gray;

        /* store it in the cache                                            */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
            grow_seq_array(cache, cache_cap, new_cap, &cache);
            cache_cap = new_cap;
        }
        cache[cache_len++] = cur[0];

        /* remaining entries of the row                                    */
        if (r > 0) {
            for (idx_t k = 1; k <= r; ++k) {
                cur[k] = cur[k-1] ^ prev_row[k-1];
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
                    grow_seq_array(cache, cache_cap, new_cap, &cache);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cur[k];
            }
        }

        /* discard old previous row                                       */
        free_seq_array(prev_row);
        prev_row = cur;
        prev_row_len = rsize;
        max_row = r;
    }
}

/* required public function */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[(size_t)n];
}
