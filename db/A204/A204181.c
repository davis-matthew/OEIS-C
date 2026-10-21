/*
 * A204181  Symmetric matrix based on f(i,j) defined by f(i,1)=f(1,j)=1;
 *          f(i,i)= 2i-1; f(i,j)=0 otherwise; by antidiagonals.
 * https://oeis.org/A204181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) gives the entry of the infinite symmetric matrix M defined
 *            by f(i,1)=f(1,j)=1, f(i,i)=2*i-1, f(i,j)=0 otherwise, read in
 *            antidiagonal order. For each n≥1, a(n) is the value at position
 *            (i,j) where i = pos+1, j = d+1-i, d is the smallest integer with
 *            d*(d+1)/2 ≥ n, and pos = n - (d-1)*d/2 - 1...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed */

#pragma once

/* Helper: find the diagonal d (≥1) such that d*(d+1)/2 ≥ n. */
static idx_t find_diagonal(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* grow hi until it covers n (safe for 64‑bit using __int128) */
    while ( ((__int128)hi * (hi + 1)) / 2 < n )
        hi <<= 1;

    /* binary search between lo and hi */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ( ((__int128)mid * (mid + 1)) / 2 >= n )
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;   /* lo == hi */
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* Determine diagonal number d. */
    idx_t d = find_diagonal(n);

    /* Number of elements before this diagonal. */
    idx_t prev = ((idx_t)(d - 1) * d) / 2;

    /* Position within the diagonal (0‑based). */
    idx_t pos = n - prev - 1;

    /* Coordinates (i,j) for this position. */
    idx_t i = pos + 1;
    idx_t j = d + 1 - i;

    /* Compute matrix entry according to definition. */
    if (i == 1 || j == 1)
        return (seq_t)1;

    if (i == j)
        return (seq_t)(2 * i - 1);

    return (seq_t)0;
}
