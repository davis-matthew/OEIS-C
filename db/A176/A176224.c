/*
 * A176224  A symmetrical triangle sequence: T(n, k) = q^k + q^(n-k) - q^n,
 *          with q=2.
 * https://oeis.org/A176224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = 2^k + 2^(r-k) - 2^r where r is the row index satisfying
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2 is the
 *            column index in that row
 * Range:     i8 n <= 37, i16 n <= 137, i32 n <= 529, i64 n <= 2081,
 *            i128 n <= 5150, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static idx_t _row_of(idx_t n)
{
    /* Find the largest r such that r*(r+1)/2 <= n. */
    idx_t low = 0;
    idx_t high = 1;
    /* Exponentially increase high until it is large enough. */
    while (((unsigned __int128)(high + 1) * (high + 2)) / 2 <= n)
        high <<= 1;
    /* Binary search between low and high. */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k for the nth element of the triangle */
    idx_t r = _row_of(n);
    idx_t tri = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - tri;   /* 0‑based column index */

    /* Compute 2^k + 2^(r-k) - 2^r using the provided pow2 helper */
    seq_t term1 = pow2(k);
    seq_t term2 = pow2(r - k);
    seq_t term3 = pow2(r);
    return term1 + term2 - term3;
}
