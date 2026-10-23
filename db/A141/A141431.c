/*
 * A141431  Triangle T(n,k) = (k-1)*(3*n-k+1), read by rows.
 * https://oeis.org/A141431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A141431: Triangle T(n,k) = (k-1)*(3*n-k+1), read by rows.
   Sequence a(n) is the n-th element of the flattened triangle (offset 1).
   For n >= 1:
       r = smallest integer with r*(r+1)/2 >= n          // row
       c = n - (r-1)*r/2                                 // column (1‑based)
       a(n) = (c-1)*(3*r - c + 1)
   This implementation uses a binary search to find r in O(log n). */

#include <stdint.h>

/* Return the n-th term of OEIS A141431.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1, so n=0 is undefined; return 0 */
        return 0;

    /* Binary search for the smallest r with r*(r+1)/2 >= n. */
    uint64_t low  = 1;
    uint64_t high = 1;
    while ((__int128)high * (high + 1) / 2 < n) {
        /* double the upper bound, but avoid overflow */
        if (high > UINT64_MAX >> 1) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }

    uint64_t r = low;                           /* row number (1‑based) */

    /* Compute c = n - (r-1)*r/2. */
    uint64_t prev_total = (uint64_t)((__int128)(r - 1) * r / 2);
    uint64_t c = n - prev_total;                /* column (1‑based) */

    /* Compute the triangle value: (c-1)*(3*r - c + 1). */
    seq_t term = (seq_t)(c - 1) * ( (seq_t)(3 * r - c + 1) );
    return term;
}
