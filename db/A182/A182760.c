/*
 * A182760  Beatty sequence for (3 + 5^(-1/2))/2.
 * https://oeis.org/A182760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((15*n + floor(sqrt(5*n*n))) / 10)) * Equivalent to
 *            a(n) = floor(r*n) with r = (3 + 5^(-1/2))/2
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 128‑bit unsigned integer.
 * Returns floor(sqrt(x)). */
static inline unsigned long long isqrt_u128 (unsigned __int128 x)
{
    unsigned long long lo = 0, hi = 0xFFFFFFFFFFFFFFFFULL;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Beatty sequence for r = (3 + 5^(-1/2))/2. */
seq_t a(idx_t n)
{
    /* n >= 1; behaviour below 1 is irrelevant */
    unsigned __int128 n128 = n;
    unsigned __int128 prod = n128 * n128 * (unsigned __int128)5;   /* 5*n^2 */
    unsigned long long sq = isqrt_u128(prod);                      /* floor(sqrt(5*n^2)) */
    unsigned __int128 num = n128 * (unsigned __int128)15 + (unsigned __int128)sq;
    return (seq_t)(num / (unsigned __int128)10);                   /* floor((15*n + sqrt)/10) */
}
