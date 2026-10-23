/*
 * A126217  Triangle read by rows: T(n,k) is the number of 321-avoiding
 *          permutations of {1,2,...,n} having longest increasing subsequence
 *          of length k (0<=k<=n).
 * https://oeis.org/A126217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = the n-th entry in the row‑wise concatenation of the
 *            triangle * T(n,k) where * * T(n,k) = ((2*k - n + 1) * C(n+1,
 *            n-k) / (n+1))^2 for floor((n+1)/2) ≤ k ≤ n * = 0 otherwise * *
 *            The rows are indexed from 0 and each row has length n+1. * * The
 *            function locates the row r and its column offset...
 * Range:     u8 n <= 39, u16 n <= 84, u32 n <= 242, u64 n <= 760,
 *            u128 n <= 2592, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* --- locate the row r that contains position n --- */
    idx_t low = 0, high = 1;
    /* double high until the triangle number exceeds n */
    while (( (__uint128_t)high * (high + 1) ) / 2 <= n)
        high <<= 1;
    /* binary search for the greatest r with r*(r+1)/2 <= n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ( ( (__uint128_t)mid * (mid + 1) ) / 2 <= n )
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t row_start = (idx_t)( ( (__uint128_t)r * (r + 1) ) / 2 );
    idx_t k = n - row_start; /* column index within the row */

    /* --- evaluate T(r,k) --- */
    idx_t min_k = (r + 1) / 2;          /* floor((r+1)/2) */
    if (k < min_k)
        return (seq_t)0;                /* outside the non‑zero range */

    /* binomial(r+1, k+1) == C(r+1, r-k)       */
    seq_t bin = binom((idx_t)(r + 1), (idx_t)(k + 1));

    /* (2*k - r + 1) is guaranteed ≥ 1 in this range */
    seq_t factor = (seq_t)( (seq_t)(2 * k - r + 1) );
    seq_t mult   = (factor * bin) / (seq_t)(r + 1);

    return mult * mult;
}
