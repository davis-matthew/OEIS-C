/*
 * A195916  Table with T(n,n) = n, T(n,k) = xor(T(n-1,k-1), T(n-1,k)).
 * https://oeis.org/A195916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A195916
 *  a(n) = T(n_row, k) where (n_row, k) are obtained by reading the
 *  triangle row‑by‑row:
 *  T(1,1) = 1
 *  T(n,0) = 0,  T(n,n) = n
 *  T(n,k) = T(n−1,k−1) xor T(n−1,k)   (1 ≤ k < n)
 *  a(1) is the first entry of the triangle (row 1).
 *  The function builds the triangle incrementally, caching values so
 *  that repeated calls are efficient.  It works for any unsigned
 *  seq_t (uint8_t … uint64_t, __uint128_t, arbitrary‑precision class).
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_cache      = NULL;   /* cached a‑values */
static size_t a_cache_len  = 0;      /* number of cached values   */
static size_t a_cache_cap  = 0;      /* allocated capacity        */

static seq_t *prev_row     = NULL;   /* previous triangle row */
static idx_t  prev_len     = 0;      /* length of previous row   */

static idx_t  current_row  = 0;      /* rows generated so far    */

seq_t a(idx_t n)
{
    /* n ≥ 1 is guaranteed by the problem statement.  */
    if (n <= a_cache_len)                       /* already cached */
        return a_cache[n - 1];

    /* continue generating rows until we have at least n values   */
    while (a_cache_len < (size_t)n)
    {
        /* next triangle row number   */
        ++current_row;
        idx_t r = current_row;

        /* allocate storage for row r (indices 0 … r)             */
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (!row) abort();          /* memory exhausted */

        row[0] = 0;                 /* T(n,0) = 0, not stored in a() */
        row[1] = 1;                 /* T(n,1) = 1 for all n */

        if (r > 1)                  /* interior values  */
        {
            /* T(r,k) = T(r-1,k-1) xor T(r-1,k)  (2 ≤ k < r) */
            for (idx_t k = 2; k < r; ++k)
                row[k] = prev_row[k - 1] ^ prev_row[k];
        }
        row[r] = r;                 /* right boundary   */

        /* append this row to the cache      */
        for (idx_t k = 1; k <= r; ++k)
        {
            if (a_cache_len == a_cache_cap)
            {
                size_t new_cap = (a_cache_cap == 0) ? 16 : a_cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                a_cache = tmp;
                a_cache_cap = new_cap;
            }
            a_cache[a_cache_len++] = row[k];
        }

        /* release previous row and keep the current one   */
        if (prev_row) free(prev_row);
        prev_row = row;
        prev_len = r;
    }

    return a_cache[n - 1];
}
