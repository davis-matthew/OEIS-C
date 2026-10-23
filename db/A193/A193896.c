/*
 * A193896  Mirror of the triangle A193895.
 * https://oeis.org/A193896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = w(r, k) where r = floor((sqrt(8*n+1)-1)/2) and k = r - (n
 *            - r*(r+1)/2). The triangle entries are w(n,k) = (n - k + 1)*(k +
 *            1)*(2*n - k)/2 (for n>=1), with w(0,0)=1. a(n) is the element at
 *            row r, column k of this triangle after the row is reversed. The
 *            implementation is O(log n) in time and...
 * Range:     u8 n <= 59, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 (row 0). */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       Use binary search to avoid overflow and keep complexity O(log n). */
    idx_t low = (idx_t)0;
    /* Rough upper bound using double arithmetic; add a safety margin. */
    long double approx = (sqrt((long double)8 * n + 1) - 1) / 2;
    idx_t high = (idx_t)approx + (idx_t)2;
    if (high > n) high = n;   /* row index never exceeds n */

    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);  /* ceil midpoint */
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Compute position within the row */
    idx_t cum_before = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t pos_in_row = n - cum_before;        /* 0 <= pos_in_row <= r */
    idx_t k = r - pos_in_row;                 /* column index in raw row */

    /* Compute w(r,k) using the derived formula.
       Use 128‑bit intermediate to avoid overflow during multiplication
       and to keep the code working for all integer‑size seq_t. */
    seq_t part1 = (seq_t)(r - k + 1);        /* (n - k + 1) */
    seq_t part2 = (seq_t)(k + 1);            /* (k + 1) */
    seq_t part3 = (seq_t)(2 * (seq_t)r - k); /* (2*n - k) */

    unsigned __int128 prod = (unsigned __int128)part1 *
                             (unsigned __int128)part2 *
                             (unsigned __int128)part3;
    prod /= 2;

    return (seq_t)prod;
}
