/*
 * A204177  Symmetric matrix based on f(i,j)=(1 if i=1 or j=1 or i=j, and 0
 *          otherwise), by antidiagonals.
 * https://oeis.org/A204177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the element at the n-th position in the antidiagonal
 *            reading of the infinite symmetric matrix M defined by f(i,j) = 1
 *            when i==1 or j==1 or i==j, otherwise 0; otherwise 0. For each n
 *            >= 1, we find the corresponding anti‑diagonal s and the
 *            coordinates (i,j) of the n-th entry on that dia...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: smallest s >= 2 with s*(s-1)/2 >= n   (O(log s)). */
static inline idx_t triangular_thres(idx_t n)
{
    /* Find an upper bound via doubling. */
    idx_t hi = 2;
    while ((unsigned __int128)hi * (hi - 1) / 2 < n)
        hi <<= 1;                     // hi will stay comfortably within 64‑bit

    /* Binary search between hi/2 and hi. */
    idx_t lo = hi >> 1;
    if (lo == 0) lo = 1;               // safety for n==0, but not used
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 v = (unsigned __int128)mid * (mid - 1) / 2;
        if (v >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)                // n must be >= 1; return 0 for safety
        return (seq_t)0;

    /* Find the anti‑diagonal number s where the n-th term lies. */
    idx_t s = triangular_thres(n);

    /* Compute the index t (0‑based) inside this anti‑diagonal. */
    unsigned __int128 prev_cum = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t t = (idx_t)(n - (idx_t)prev_cum - 1);   // t < s-1

    /* Coordinates along the anti‑diagonal: i increases. */
    idx_t i = t + 1;
    idx_t j = s - i;

    /* Determine if the matrix entry is 1. */
    uint8_t val = (i == 1 || j == 1 || i == j) ? 1 : 0;
    return (seq_t)val;
}
