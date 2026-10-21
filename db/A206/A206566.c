/*
 * A206566  Triangular array: T(i,j) = number of terms common to the binary
 *          expansions of i+1 and j, for j=1,2,3,...,i; i=1,2,3,...
 * https://oeis.org/A206566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = popcount( (i+1) & j ) where i = smallest integer with
 *            i(i+1)/2 >= n and j = n - i(i-1)/2
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   _popcount_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes for basic operations */
#include <stdint.h>
#include <stddef.h>
#include "../utils/_popcount_seq.h"

/* Helper function to compute the row index i for a given linear index n. */
static inline idx_t _row_index(idx_t n)
{
    idx_t lo = 1;
    idx_t hi = n;            /* upper bound is safe for n up to 2^64-1 */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        /* Use 128-bit arithmetic to avoid overflow in mid*(mid+1)/2 */
        __int128 tri = (__int128)mid * (__int128)(mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the row i and column j for index n */
    idx_t i = _row_index(n);
    seq_t i1 = (seq_t)(i + 1);                  /* i+1  */
    idx_t prev_sum = (idx_t)(((__int128)i * (__int128)(i - 1)) / 2);
    idx_t j = n - prev_sum;                      /* 1 <= j <= i */

    /* Compute number of common 1-bit positions between i+1 and j */
    seq_t common = i1 & (seq_t)j;
    return (seq_t)_popcount_seq(common);
}
