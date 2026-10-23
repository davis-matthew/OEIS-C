/*
 * A049735  Array T(i,j) is the number of lattice points (x,y) in circle with
 *          radius (0,0)-to-(i,j), read by antidiagonals.
 * https://oeis.org/A049735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of lattice points (x,y) with x²+y² ≤ i²+j², where
 *            (i,j) is the n‑th point in the antidiagonal enumeration (i+j
 *            fixed, i+j != 0 only once)
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Integer triangular number: T(r) = r(r+1)/2, computed safely with 128‑bit */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
}

/* Find the unique d such that T(d) ≤ n < T(d+1) */
static inline idx_t antidiagonal_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;                 /* T(high) ≥ n for all high ≥ n */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t t = triangular(mid);
        if (t <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* last d with T(d) ≤ n */
}

/* Count lattice points inside the circle of radius sqrt(R).           */
static seq_t points_within(idx_t i, idx_t j)
{
    seq_t R = (seq_t)i * (seq_t)i + (seq_t)j * (seq_t)j;
    seq_t m_seq = floor_sqrt(R);
    idx_t m = (idx_t)m_seq; /* m is at most sqrt(R) and fits in idx_t for n ≤ 10000 */

    seq_t count = 0;
    for (idx_t x = 0; x <= m; ++x) {
        seq_t x_sq = (seq_t)x * (seq_t)x;
        seq_t y_max = floor_sqrt(R - x_sq);
        seq_t contrib = 2 * y_max + 1; /* number of y for this |x| */

        if (x == 0)
            count += contrib; /* only one x value (x=0) */
        else
            count += 2 * contrib; /* x and -x */
    }
    return count;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Determine the antidiagonal (i,j) corresponding to index n. */
    idx_t d = antidiagonal_index(n);   /* d = i + j */
    idx_t start = triangular(d);       /* index of first point on this diagonal */
    idx_t offset = n - start;          /* position along the diagonal */
    idx_t i = offset;                  /* row index */
    idx_t j = d - offset;              /* column index */

    return points_within(i, j);
}
