/*
 * A101870  Row 3 of A101866.
 * https://oeis.org/A101870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5909, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A101870
   Formula: a(n) = 3*n + 5*ceil(n*(1 + sqrt(5))/2)  for n ≥ 1
   Using the identity ceil(n*φ) = floor((n + floor(n*sqrt(5)))/2) + 1
   where φ = (1+sqrt(5))/2.  */
#include <stdint.h>

/* Integer square root of an unsigned 128‑bit integer.
   Returns floor(sqrt(x)).  Works with any x that fits in unsigned __int128. */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = x, mid, sq;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined offset, safe guard */
        return 0;

    /* 5 * n * n fits in unsigned __int128 for normal usage. */
    unsigned __int128 prod = (unsigned __int128)n * n * (unsigned __int128)5;

    /* floor( n * sqrt(5) ) = floor( sqrt(5 * n^2) ) */
    unsigned __int128 s = isqrt128(prod);

    /* floor( n * φ ) = floor( ( n + floor(n*sqrt(5)) ) / 2 ) */
    unsigned __int128 t = (unsigned __int128)n + s;
    unsigned __int128 floor_n_phi = t >> 1;          /* divide by 2, integer */

    /* ceil( n * φ ) = floor(n*φ) + 1            (n*φ is irrational) */
    unsigned __int128 ceil_n_phi = floor_n_phi + 1;

    /* a(n) = 3*n + 5*ceil(n*φ) */
    unsigned __int128 result = (unsigned __int128)3 * n + (unsigned __int128)5 * ceil_n_phi;

    return (seq_t)result;
}
