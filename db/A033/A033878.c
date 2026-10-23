/*
 * A033878  Triangular array associated with Schroeder numbers.
 * https://oeis.org/A033878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is generated from the transposed Schröder triangle. The
 *            triangle is constructed by the recurrence T(m,0) = 1 T(m,1) =
 *            2*m-1 T(m,k) = T(m,k-1) + T(m-1,k) + T(m-1,k-1) for 2 ≤ k ≤ m-1
 *            T(m,m) = S(m-1) (the (m‑1)‑th large Schröder number) where
 *            S(0)=1, S(m)=∑_{k=0}^{m} T(m,k). The terms a(n) are...
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;            /* flattened triangle */
    static size_t cache_size = 0;          /* number of terms stored */

    static seq_t *prevRow = NULL;          /* previous row values   */
    static size_t prevLen = 0;             /* length of previous row */
    static seq_t prevSum = 0;              /* S(m-1) for next row  */
    static idx_t nextRow = 0;              /* next row index to compute */

    /* first time: initialise row 0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* out of memory: return 0 */
        cache[0] = (seq_t)1;
        cache_size = 1;

        prevRow = (seq_t *)malloc(sizeof(seq_t));
        if (!prevRow) return (seq_t)0;          /* out of memory */
        prevRow[0] = (seq_t)1;
        prevLen = 1;
        prevSum = (seq_t)1;    /* S0 = 1 */
        nextRow = 1;           /* next row to compute is 1 */
    }

    /* extend cache until it covers requested index n */
    while (cache_size <= (size_t)n) {
        idx_t m = nextRow;               /* index of the new row   */
        size_t len = (size_t)m + 1;      /* number of elements in row */

        /* allocate space for current row */
        seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cur) return (seq_t)0;       /* out of memory */

        cur[0] = (seq_t)1;
        if (m >= 1) {
            cur[1] = (seq_t)(2 * m - 1);
        }
        for (idx_t k = 2; k <= m - 1; ++k) {
            cur[k] = cur[k - 1] + prevRow[k] + prevRow[k - 1];
        }
        /* last element is the previous Schröder number */
        cur[m] = prevSum;

        /* append this row to the flattened cache */
        seq_t *new_cache = (seq_t *)realloc(cache,
                            (cache_size + len) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* out of memory */
        cache = new_cache;
        for (size_t i = 0; i < len; ++i) {
            cache[cache_size + i] = cur[i];
        }
        cache_size += len;

        /* compute new Schröder number for the next row */
        seq_t sum = 0;
        for (size_t i = 0; i < len; ++i) {
            sum += cur[i];
        }

        /* update state for next iteration */
        if (prevRow) free(prevRow);
        prevRow = cur;
        prevLen = len;
        prevSum = sum;
        ++nextRow;
    }

    return cache[(size_t)n];
}
