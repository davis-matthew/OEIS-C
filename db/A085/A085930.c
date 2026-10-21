/*
 * A085930  Triangle read by rows in which row n contains n smallest (> 0)
 *          numbers which when incremented by n yield a triangular number.
 * https://oeis.org/A085930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 9847, u32 n <= 9847, u64 n <= 9847,
 *            u128 n <= 9847, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained from the triangle in which row r contains the
   r smallest positive integers k such that k+r is a triangular number.
   For a given index n we find the row r with
      (r−1)·r/2 < n ≤ r·(r+1)/2
   and the position offset = n−(r−1)·r/2−1 within that row.
   The offset‑th element of row r is T(t)−r where
      t = floor((√(8r+1)−1)/2)+1+offset
   and T(t)=t·(t+1)/2. */

/* Basic integer types are supplied by the build */
#include "../utils/floor_sqrt.h"

/* Find the row r such that r(r−1)/2 < n ≤ r(r+1)/2 */
static inline idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = n;          /* hi ≥ required row */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum < n) lo = mid + 1;
        else          hi = mid;
    }
    return lo;
}

/* Compute triangular number T(t) = t·(t+1)/2 using 128‑bit to avoid overflow */
static inline seq_t triangular(idx_t t)
{
    unsigned __int128 prod = (unsigned __int128)t * (t + 1) / 2;
    return (seq_t)prod;
}

/* Return the n-th term of A085930 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset before sequence start */

    /* Identify the row and offset within the row */
    idx_t r       = find_row(n);
    unsigned __int128 prev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev      = (idx_t)prev128;
    idx_t offset    = n - prev - 1;      /* 0‑based position in this row */

    /* t0 = floor((√(8r+1)−1)/2) gives the largest t with T(t) ≤ r */
    seq_t val   = (seq_t)r * 8 + 1;
    seq_t sqrtV = floor_sqrt(val);
    idx_t t0    = (idx_t)((sqrtV - 1) / 2);

    /* t_start = t0 + 1 gives the first triangular index for this row */
    idx_t t_start = t0 + 1;
    idx_t t      = t_start + offset;

    /* Desired value is T(t) – r */
    seq_t term = triangular(t);
    return term - (seq_t)r;
}
