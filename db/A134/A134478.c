/*
 * A134478  Triangle read by rows, T(0,0) = 1; n-th row = (n+1) terms of n,
 *          n+1, n+2, ...
 * https://oeis.org/A134478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, tabl
 * Formula:   a(n) = 1 if n = 0 otherwise a(n) = n - r*(r+1)/2 + r, where r =
 *            floor( (sqrt(8*n+1)-1)/2 ) (row index in the triangle)
 * Range:     u8 n <= 5049, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(v)) for an unsigned 64‑bit integer without overflow.
   Uses only integer arithmetic. */
static inline idx_t floor_sqrt_u64(idx_t v)
{
    idx_t low  = 0;
    idx_t high = v;
    idx_t res  = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            res = 0;
            low = 1;
            continue;
        }
        /* mid*mid <= v is equivalent to mid <= v/mid, no overflow */
        if (mid <= v / mid) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;                     /* irregular first element */
    }

    /* Compute r = floor((sqrt(8*n+1)-1)/2) */
    idx_t tmp = n * (idx_t)8 + (idx_t)1;     /* 8*n+1  (fits for the test range) */
    idx_t s   = floor_sqrt_u64(tmp);
    idx_t r   = (s - (idx_t)1) >> 1;         /* (s-1)/2 */

    /* Offset of the first element of row r */
    idx_t offset = r * (r + 1) / 2;

    idx_t k = n - offset;                    /* column index in the row */

    /* a(n) = r + k  (for r >= 1) */
    return (seq_t)(r + k);
}
