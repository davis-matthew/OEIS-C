/*
 * A155999  Triangle T(n, k) = (-1)^n*StirlingS2(n, k)*StirlingS2(n, n-k),
 *          read by rows.
 * https://oeis.org/A155999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 30, i32 n <= 69, i64 n <= 159,
 *            i128 n <= 390, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A155999: T(n,k)=(-1)^n * StirlingS2(n,k) * StirlingS2(n,n-k) read by rows. Offset 0. */

#include <stdlib.h>

/* Utility for allocating and freeing a row of Stirling numbers */
#ifdef __cplusplus
#define ALLOC_ROW(len)  new seq_t[(len)+1]
#define FREE_ROW(ptr)   delete[] (ptr)
#else
#define ALLOC_ROW(len)  ((seq_t*)calloc((size_t)((len)+1), sizeof(seq_t)))
#define FREE_ROW(ptr)   free((ptr))
#endif

/* Return the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t _row_of(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main function as required */
seq_t a(idx_t n)
{
    /* Identify row r and column k for the flattened index n */
    idx_t r = _row_of(n);
    idx_t base = r * (r + 1) / 2;
    idx_t k = n - base;

    /* Build the Stirling numbers of the second kind for row r */
    seq_t *row = ALLOC_ROW(r);
    if (!row) return 0;  /* allocation failure, return 0 as safe value */

    row[0] = 1;          /* S(0,0) */
    for (idx_t m = 1; m <= r; ++m) {
        row[m] = 1;                 /* S(m,m) = 1 */
        for (idx_t j = m - 1; j > 0; --j) {
            /* S(m,j) = j*S(m-1,j) + S(m-1,j-1) */
            row[j] = (seq_t)j * row[j] + row[j - 1];
        }
        row[0] = 0;                 /* S(m,0) = 0 for m>0 */
    }

    seq_t sk  = row[k];
    seq_t srk = row[r - k];
    seq_t prod = sk * srk;
    if (r & 1) prod = -prod;        /* multiply by (-1)^r */

    FREE_ROW(row);
    return prod;
}
