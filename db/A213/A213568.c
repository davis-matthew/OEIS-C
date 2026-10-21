/*
 * A213568  Rectangular array: (row n) = b**c, where b(h) = 2^(h-1), c(h) =
 *          n-1+h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^c*(r+1) - (r + c + 1), where (r,c) are the row and
 *            column of the n‑th entry in the rectangular array read by
 *            falling antidiagonals. r = offset in diagonal d, c = d - offset,
 *            and d is the smallest integer with (d-1)*d/2 >= n
 * Range:     u8 n <= 28, u16 n <= 120, u32 n <= 496, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Find the diagonal index d that contains the n-th term. */
    idx_t low = 1, high = 1;
    /* Increase high until it satisfies the condition. */
    while (( (__int128)(high - 1) * high ) / 2 < n)
        high <<= 1;
    /* Binary search for minimal d. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__int128)(mid - 1) * mid ) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;   /* d >= 2 for all n >= 1 */

    /* Number of terms before this diagonal. */
    idx_t prev = (idx_t)( ( (__int128)(d - 2) * (d - 1) ) / 2 );

    /* Position inside the diagonal (1‑based). */
    idx_t offset = n - prev;          /* 1 <= offset <= d-1 */

    idx_t r = offset;                  /* row index (1‑based)    */
    idx_t c = d - offset;              /* column index (1‑based) */

    /* Compute the value T(r,c). */
    seq_t pow2c = pow2(c);             /* 2^c */
    seq_t term   = pow2c * ( (seq_t)r + 1 )
                  - ( (seq_t)r + (seq_t)c + 1 );

    return term;
}
