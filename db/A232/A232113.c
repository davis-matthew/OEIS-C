/*
 * A232113  a(n) is the Manhattan distance between n and 2*n in a square
 *          spiral of positive integers with 1 at the center.
 * https://oeis.org/A232113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence A232113:
   a(n) = Manhattan distance between n and 2*n in the Ulam square spiral
   (coordinates are computed from the layer number of each integer). */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) for 0 <= n < 2^64.
   Implemented with a binary search that uses only integer operations. */
static inline uint64_t floor_sqrt_u64(uint64_t n) {
    uint64_t low = 0, high = n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)   /* mid*mid <= n  (avoid overflow with division) */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute the (x,y) coordinates of integer k in the standard Ulam spiral.
   The spiral starts with 1 at (0,0).  Coordinate differences are signed
   64‑bit; types seq_t and idx_t are provided by the build. */
static void coord(idx_t k, long long *x, long long *y)
{
    if (k == 1) {
        *x = *y = 0;
        return;
    }

    /* Layer m: numbers from (2m-1)^2+1 to (2m+1)^2 lie on it.
       For k > 1,  m = ceil((sqrt(k)-1)/2) = (floor_sqrt(k-1)+1)/2. */
    uint64_t m = (floor_sqrt_u64(k - 1) + 1) >> 1;   /* divide by 2 */

    uint64_t side     = 2 * m;          /* side length of the square ring */
    uint64_t prev_len = 2 * m - 1;      /* side length of the previous ring */
    uint64_t base     = prev_len * prev_len + 1;  /* first number in this ring */
    uint64_t t        = k - base;       /* offset within the ring (0 … 8m-1) */

    if (t < side) {                                 /* moving up */
        *x = (long long)m;
        *y = -(long long)m + 1 + (long long)t;
    } else if (t < side * 2) {                      /* moving left */
        *x = (long long)(m - 1 - (t - side));
        *y = (long long)m;
    } else if (t < side * 3) {                      /* moving down */
        *x = -(long long)m;
        *y = (long long)(m - 1 - (t - side * 2));
    } else {                                         /* moving right */
        *x = -(long long)m + 1 + (long long)(t - side * 3);
        *y = -(long long)m;
    }
}

/* Return a(n): Manhattan distance between the coordinates of n and 2*n. */
seq_t a(idx_t n)
{
    idx_t k1 = n;
    idx_t k2 = n * 2;          /* the tests use values < 2^63, so wrap‑around
                                 will not occur in practice */

    long long x1, y1, x2, y2;
    coord(k1, &x1, &y1);
    coord(k2, &x2, &y2);

    long long dx = x1 - x2;
    if (dx < 0) dx = -dx;
    long long dy = y1 - y2;
    if (dy < 0) dy = -dy;

    return (seq_t)(dx + dy);
}
