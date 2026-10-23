/*
 * A177696  Symmetrical triangle read by rows: T(n, k) = m*(T(n-1, k-1) +
 *          T(n-1, k)), where T(n, 1) = T(n, n) = n, and m = 2.
 * https://oeis.org/A177696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 58, u32 n <= 178, u64 n <= 609,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A177696 – the flattened, symmetrically‑filled triangle
   with recurrence
   T(n,k) = 2·(T(n‑1,k‑1) + T(n‑1,k)),   T(n,1) = T(n,n) = n.
   a(n) is the n‑th element when the triangle is read row by row
   (row n has length n).  The routine builds rows on demand and stores
   them in a flat cache so that any previously requested index can be
   answered in O(1). */

#include <stdlib.h>
#include <stdint.h>

/* Return the triangular number for n: 1+2+...+n = n(n+1)/2. */
static inline idx_t triangular(idx_t n)
{
    return n * (n + 1) / 2;
}

/* Find the row r such that triangular(r‑1) < n <= triangular(r). */
static idx_t row_of(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 1;
    while (triangular(high) < n)
        high <<= 1;                     /* double until sufficient */

    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ---------- Static cache for all computed rows ---------- */
static seq_t *cache      = NULL;   /* flat array: a(1) is cache[0]   */
static idx_t  cache_cap  = 0;      /* allocated capacity            */
static idx_t  rows_done  = 0;      /* number of rows fully computed */

/* Ensure all rows up to and including r are computed and stored. */
static void ensure_rows(idx_t r)
{
    if (rows_done >= r)
        return;

    /* Allocate/resize cache if necessary */
    idx_t needed = triangular(r);
    if (needed > cache_cap)
    {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cache = tmp;
        cache_cap = needed;
    }

    /* First row special‑case */
    if (rows_done == 0)
    {
        cache[0] = (seq_t)1;        /* T(1,1) = 1 */
        rows_done = 1;
    }

    /* Build successive rows */
    for (idx_t row = rows_done; row < r; ++row)
    {
        idx_t next       = row + 1;                /* new row number      */
        idx_t offset_prev = triangular(row - 1);  /* start of previous row */
        idx_t offset_cur  = triangular(row);      /* start of new row    */

        /* Boundary values */
        cache[offset_cur]                     = (seq_t)next;                     /* k = 1   */
        cache[offset_cur + next - 1]           = (seq_t)next;                     /* k = next */

        /* Interior values */
        for (idx_t k = 2; k <= next - 1; ++k)
        {
            seq_t a = cache[offset_prev + k - 2];
            seq_t b = cache[offset_prev + k - 1];
            cache[offset_cur + k - 1] = (a + b) * ((seq_t)2);
        }
    }

    rows_done = r;
}

/* Public routine: a(n) – nth term of sequence A177696 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 */

    idx_t r = row_of(n);
    ensure_rows(r);
    return cache[n - 1];          /* flattened index is just n-1       */
}
