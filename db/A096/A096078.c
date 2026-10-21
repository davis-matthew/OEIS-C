/*
 * A096078  Triangle read by rows: T(n,k) = (k+1)*T(n-1,k) + (n-k+1)*T(n,k-1).
 * https://oeis.org/A096078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A096078
 *  Triangle read by rows:
 *      T(0,0)=1
 *      T(n,k) = (k+1)*T(n-1,k) + (n-k+1)*T(n,k-1)        (0 ≤ k ≤ n)
 *  a(n) = the n‑th element of the triangle when flattened in row‑major order.
 *
 *  The function keeps a static cache of the flattened triangle and a
 *  copy of the previous row to generate new rows on demand.  It works
 *  for all integer build types of seq_t and for an arbitrary‑precision
 *  seq_t (C++ class) because all allocations are cast to seq_t*.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* ---- static cache ----------------------------------------------------- */
    static seq_t *cache      = (seq_t*)NULL;   /* flattened triangle values */
    static size_t cacheSize  = 0;              /* number of stored values   */
    static size_t cacheCap   = 0;              /* capacity of the cache     */

    static seq_t *prevRow    = (seq_t*)NULL;   /* T(n-1,*) row */
    static size_t prevSize   = 0;              /* length of prevRow         */
    static idx_t  prevIdx    = 0;              /* row index of prevRow (n-1)*/

    /* helper to grow the cache array when needed */
    if (cacheSize <= (size_t)n) {
        size_t newCap = cacheCap ? cacheCap : 1;
        while (newCap <= (size_t)n) newCap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        cacheCap = newCap;
    }

    /* initialise on the very first call */
    if (cacheSize == 0) {
        cacheSize = 1;
        cache[0] = (seq_t)1;                  /* T(0,0) = 1 */

        prevSize = 1;
        prevRow  = (seq_t*)malloc(sizeof(seq_t));
        if (!prevRow) return (seq_t)0;
        prevRow[0] = (seq_t)1;                /* row 0 */

        prevIdx = 0;
    }

    /* if requested value already cached, return it */
    if ((size_t)n < cacheSize) return cache[(size_t)n];

    /* build successive rows until the requested index is cached */
    for (idx_t r = prevIdx + 1; ; ++r) {
        size_t currSize = (size_t)(r + 1);   /* number of elements in row r */
        seq_t *currRow = (seq_t*)malloc(currSize * sizeof(seq_t));
        if (!currRow) return (seq_t)0;

        for (idx_t k = 0; k <= r; ++k) {
            seq_t up    = (k < prevSize) ? prevRow[(size_t)k] : (seq_t)0;
            seq_t left  = (k > 0)        ? currRow[(size_t)(k - 1)] : (seq_t)0;
            currRow[(size_t)k] =
                (seq_t)(k + 1) * up +  (seq_t)(r - k + 1) * left;
        }

        /* append the new row into the cache */
        size_t start = cacheSize;
        cacheSize += currSize;
        if (cacheSize > cacheCap) {          /* grow cache if necessary */
            size_t newCap = cacheCap;
            while (newCap < cacheSize) newCap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, newCap * sizeof(seq_t));
            if (!tmp) { free(currRow); return (seq_t)0; }
            cache = tmp;
            cacheCap = newCap;
        }
        for (size_t i = 0; i < currSize; ++i) cache[start + i] = currRow[i];

        /* free the previous row and store the current one for the next iteration */
        free(prevRow);
        prevRow   = currRow;
        prevSize  = currSize;
        prevIdx   = r;

        /* if the required index is now in the cache, return it */
        if ((size_t)n < cacheSize) return cache[(size_t)n];
    }
}
