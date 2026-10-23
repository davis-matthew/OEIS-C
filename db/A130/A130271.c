/*
 * A130271  Triangle read by rows: A051340^2.
 * https://oeis.org/A130271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A130271: Triangle read by rows – A051340²
 *
 *  Elements of the triangle:
 *      for row r ≥ 1, column c (1 ≤ c ≤ r)
 *
 *          a(n) = (c < r) ? (2*r-1) : (r*r)
 *
 *  The linear index n corresponds to the pair (r,c) where
 *      T(r-1) < n ≤ T(r)   with   T(r) = r*(r+1)/2 .
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer (floor) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32);   /* 2^32 > sqrt(ULLONG_MAX) */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 – 0 is unused */

    /* find the row r: smallest r with r*(r+1)/2 ≥ n */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) >> 1;
    if (((unsigned __int128)r * (r + 1)) / 2 < n) ++r;

    /* previous triangular number T(r‑1) = r*(r‑1)/2 */
    idx_t prev_tri = ((unsigned __int128)r * (r - 1)) / 2;
    idx_t c = n - prev_tri;           /* 1‑based column within the row */

    if (c == r)                       /* diagonal entry */
        return (seq_t)r * (seq_t)r;   /* r² */
    else
        return (seq_t)(2 * r - 1);    /* off‑diagonal entries */
}
