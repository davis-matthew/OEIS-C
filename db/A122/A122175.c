/*
 * A122175  Triangle, read by rows, where T(n,k) = C( k*(k+1)/2 + n-k, n-k)
 *          for n>=k>=0.
 * https://oeis.org/A122175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = let r = ⌊(√(8 n + 1) − 1)/2⌋, k = n − r(r + 1)/2, then
 *            a(n) = binom( k(k + 1)/2 + r − k , r − k ) where binom(x,y) is
 *            binomial(x,y) (see utils/binom.h)
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include needed utilities */
#include "../utils/binom.h"

/* Integer square‑root for 64‑bit unsigned integers.
   Uses binary search, no <math.h> dependence. */
static idx_t
floor_sqrt(idx_t x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1u << 32;      /* 2^32 is > √(2^64−1) */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* ceil */
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Compute r = floor((√(8n+1)−1)/2) */
    idx_t disc = 8 * n + 1;
    idx_t r = (floor_sqrt(disc) - 1) >> 1;   /* (sqrt-1)/2 */

    /* Compute the first index of row r */
    idx_t row_start = r * (r + 1) / 2;

    /* Column within the row */
    idx_t k = n - row_start;

    /* Arguments for binomial */
    idx_t numer = k * (k + 1) / 2 + r - k;
    idx_t denom = r - k;

    return binom(numer, denom);
}
