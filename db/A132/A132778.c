/*
 * A132778  Triangle read by rows, n-1 terms of (2n - 1) followed by n.
 * https://oeis.org/A132778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of a triangle where row r (r ≥ 1)
 *            contains r‑1 copies of 2r‑1 followed by r
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only integer arithmetic; 64‑bit arithmetic guarded by
   __int128 to avoid overflow. */

#include <stdint.h>

static idx_t row_of(idx_t n)
{
    /* Find smallest r such that r*(r+1)/2 >= n using binary search. */
    idx_t low = 1, high = 1;
    /* Find an upper bound for r. */
    while (1) {
        __int128 tri = (__int128)high * (high + 1) / 2;
        if (tri >= n) break;
        high <<= 1;           /* double until we overshoot */
    }
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1)   /* sequence is 1‑based; undefined for n<1 – return 0 */
        return 0;

    idx_t r   = row_of(n);                     /* the row containing n   */
    __int128 prev_tri = (__int128)r * (r - 1) / 2; /* elements before row r */
    idx_t k = n - (idx_t)prev_tri - 1;          /* zero‑based offset in row */

    if (k < (r - 1))
        return (seq_t)(2 * r - 1);   /* entries before the last in the row */
    else
        return (seq_t)r;             /* the last entry of the row              */
}
