/*
 * A152904  Triangle read by rows: T(n,k) = A008683(n-k+1); 1<=k<=n; mu(n)
 *          "decrescendo".
 * https://oeis.org/A152904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = μ( r - ( n - (r-1)*r/2 ) + 1 ) where r =
 *            ceil((√(8*n+1)-1)/2); μ is the Möbius function
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the row number r such that r*(r+1)/2 >= n, for n>=1. */
static inline idx_t triangular_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Exponential search to find an upper bound. */
    while ( ( (__int128)high * (high + 1) / 2 ) < n )
        high <<= 1;
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__int128)mid * (mid + 1) / 2 ) >= n )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Möbius function μ(m) for unsigned integer m >= 1. */
static inline seq_t mobius(idx_t m)
{
    if (m == 1) return 1;
    idx_t t = m;
    idx_t prime_count = 0;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            ++prime_count;
            t /= p;
            if (t % p == 0)          /* square factor */
                return 0;
            /* remove remaining power of p if any */
            while (t % p == 0)
                t /= p;
        }
    }
    if (t > 1) ++prime_count;          /* remaining prime factor */
    return (prime_count & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for 0, return 0 */
    idx_t r = triangular_row(n);    /* row number */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t col = n - prev_tri;        /* column index in row */
    idx_t m = r - col + 1;           /* argument to μ */
    return mobius(m);
}
