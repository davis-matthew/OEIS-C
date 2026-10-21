/*
 * A089805  Expansion of Jacobi theta function (theta_4(q^6) -
 *          theta_4(q^(2/3)))/2/q^(2/3).
 * https://oeis.org/A089805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A089805 
   a(n) = Sum_{k>=0} (-1)^k * ( [n = 6k^2+4k] - [n = 6k^2+8k+2] ).
   For odd n the value is 0.  For even n this is equivalent to
   a(2m) = A089801(m) where A089801 is given by the same formula with
   m substituted for n.
   This implementation checks whether n matches one of the two
   quadratic forms and returns the corresponding sign. */

#include <stdint.h>

/* integer square root for 128‑bit values.  Returns floor(sqrt(x)).  */
static unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned long long low = 0;
    unsigned long long high = 1ULL << 35; /* > sqrt(2^68) */
    while (low < high) {
        unsigned long long mid = (low + high + 1) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Odd indices are zero. This also avoids unnecessary calculation. */
    if (n & 1ULL)
        return 0;

    /* D = 16 + 24*n must be a perfect square for a match to exist. */
    unsigned __int128 D = (unsigned __int128)24 * n + 16;
    unsigned long long t = isqrt128(D);
    if ((unsigned __int128)t * t != D)
        return 0;

    /* Check the two possible solutions of the quadratic equations. */

    /* 6k^2 + 4k = n  =>  k = (t - 4)/12  */
    if (t >= 4ULL && (t - 4ULL) % 12ULL == 0ULL) {
        seq_t k = (seq_t)((t - 4ULL) / 12ULL);
        return (k & 1) ? -1 : 1;       /* sign = (-1)^k */
    }

    /* 6k^2 + 8k + 2 = n  =>  k = (t - 8)/12  */
    if (t >= 8ULL && (t - 8ULL) % 12ULL == 0ULL) {
        seq_t k = (seq_t)((t - 8ULL) / 12ULL);
        return (k & 1) ? 1 : -1;       /* sign = (-1)^(k+1) */
    }

    return 0;
}
