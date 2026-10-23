/*
 * A128139  Triangle read by rows: matrix product A004736 * A128132.
 * https://oeis.org/A128139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n-th element (0‑based) of the triangle read by rows,
 *            where the triangle is given by T(r,k) = k * (1 + r - k) + 1 (0 ≤
 *            k ≤ r, r ≥ 0) The triangle rows have lengths 1,2,3,…; thus the
 *            index n is mapped to the unique pair (r,k) with S(r-1) ≤ n <
 *            S(r) where S(r) = (r+1)(r+2)/2. The impleme...
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Integer square‑root for 128‑bit values (floor).  Works for all values
   up to 2^128‑1, but r and s will always be < 2^36.                */
/* ------------------------------------------------------------------ */
static inline idx_t floor_sqrt_uint128(__uint128_t x)
{
    idx_t low = 0, high = 1;
    /* Increase high until high^2 > x, using a safe comparison */
    while ( ( __uint128_t)high * high <= x ) {
        high <<= 1;
    }
    low = high >> 1;
    while (low < high) {
        idx_t mid = low + ( (high - low + 1) >> 1 );
        if ( (__uint128_t)mid * mid <= x ) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* Compute a(n) for the triangle read by rows.                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find r: the row containing index n.
       Compute s = floor(sqrt(8*n + 1)) using 128‑bit arithmetic to avoid overflow.
       The formula derived from S(r) = (r+1)(r+2)/2 gives
           r = floor((s - 3) / 2) + 1  for s ≥ 3,
           r = 0                       for s < 3  (i.e., n = 0).   */
    __uint128_t temp = (__uint128_t)8 * n + 1;
    idx_t s = floor_sqrt_uint128(temp);

    idx_t r = (s < 3) ? 0 : ((s - 3) / 2 + 1);

    /* Compute the start index of row r:  S(r-1) = r*(r+1)/2   (valid for r ≥ 0). */
    idx_t start = (idx_t)((__uint128_t)r * (r + 1) / 2);

    idx_t k = n - start;   /* 0 ≤ k ≤ r */

    /* T(r,k) = k * (1 + r - k) + 1 */
    seq_t val = (seq_t)k * ((seq_t)(1 + r - k)) + 1;
    return val;
}
