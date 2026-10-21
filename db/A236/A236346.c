/*
 * A236346  Manhattan distances between n^2 and (n+1)^2 in a left-aligned
 *          triangle with next M natural numbers in row M: 1, 2 3, 4 5 6, 7 8
 *          9 10, etc.
 * https://oeis.org/A236346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 306, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   col_of_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = |row(n^2)-row((n+1)^2)| + |col(n^2)-col((n+1)^2)|
 * where row(k) and col(k) give the row and left‑aligned column of
 * number k in the triangle 1; 2 3; 4 5 6; 7 8 9 10; …
 * (Manhattan distance between the squares n^2 and (n+1)^2.)
 */

#include <stddef.h>
#include "../utils/col_of_square.h"

/* Integer floor square root of a 64‑bit value, using only division. */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 1, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= x / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Row of the square n^2 in the triangle. */
static inline idx_t row_of_square(idx_t n)
{
    idx_t nn = n * n;
    idx_t t   = 8 * nn + 1;          /* 8*n^2 + 1   */
    idx_t s   = floor_sqrt_u64(t);   /* floor sqrt(8*n^2+1) */
    idx_t r   = (s - 1) >> 1;        /* floor((s-1)/2) */
    /* Ensure r satisfies r*(r+1)/2 >= n^2. */
    idx_t tri = r * (r + 1) / 2;
    if (tri < nn)
        r++;
    return r;
}

/* Sequence definition. */
seq_t a(idx_t n)
{
    /* coordinates of n^2 */
    idx_t r1 = row_of_square(n);
    idx_t c1 = col_of_square(n, r1);

    /* coordinates of (n+1)^2 */
    idx_t r2 = row_of_square(n + 1);
    idx_t c2 = col_of_square(n + 1, r2);

    seq_t dr = (r1 > r2) ? (seq_t)(r1 - r2) : (seq_t)(r2 - r1);
    seq_t dc = (c1 > c2) ? (seq_t)(c1 - c2) : (seq_t)(c2 - c1);
    return dr + dc;
}
