/*
 * A115322  Triangle of coefficients of Pell polynomials.
 * https://oeis.org/A115322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = ceil((sqrt(8*n+1)-1)/2) (row number) *
 *            k = n - r*(r-1)/2 - 1 (0‑based column) * T(r,k) = 0 if r and k
 *            have the same parity * T(r,k) = 2^k * C((r+k-1)/2, k) otherwise
 *            * The triangle T(r,k) enumerated row by row gives the OEIS
 *            sequence * A115322
 * Range:     u8 n <= 42, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */

/* Find the row r such that (r-1)*r/2 < n ≤ r*(r+1)/2. */
static idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = 1;

    /* Increase hi until it bounds n */
    while (true) {
        unsigned __int128 tri = (unsigned __int128)hi * (hi + 1) / 2;
        if (tri >= n)
            break;
        if (hi > ((idx_t)-1) >> 1) {
            hi = (idx_t)-1;
            break;
        }
        hi <<= 1;
    }

    /* Binary search for the minimal hi with cumulative sum ≥ n. */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* unused for OEIS offset 1 */

    /* Determine row r and column k within that row. */
    idx_t r = find_row(n);

    unsigned __int128 start_calc = (unsigned __int128)(r - 1) * r / 2;
    idx_t start = (idx_t)start_calc + 1;
    idx_t k = n - start;            /* 0‑based column */

    /* If r and k have the same parity, the coefficient is zero. */
    if ( ((r & 1) == (k & 1)) )
        return (seq_t)0;

    /* Compute binomial part: C((r + k - 1)/2, k). */
    idx_t m = (r + k - 1) / 2;
    seq_t bin = binom(m, k);

    /* Compute 2^k. */
    seq_t pow = pow2(k);

    return bin * pow;
}
