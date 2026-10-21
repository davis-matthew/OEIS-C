/*
 * A088673  a(n) = n mod A002024(n), where A002024 is "n appears n times": 1,
 *          2, 2, 3, 3, 3, ...
 * https://oeis.org/A088673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod ceil((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 128‑bit unsigned value: floor(sqrt(x)) */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = ((unsigned __int128)1 << 64);   /* sqrt(2^128‑1) < 2^64 */
    while (lo < hi)
    {
        unsigned __int128 mid = (lo + hi + 1) >> 1;   /* (lo+hi+1)/2 */
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* sequence A088673 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute m = ceil((sqrt(8*n+1)-1)/2) */
    unsigned __int128 val   = (unsigned __int128)n * 8ULL + 1;
    unsigned __int128 d128  = isqrt128(val);
    uint64_t d              = (uint64_t)d128;
    uint64_t m              = (d - 1) / 2;          /* floor part */

    /* adjust to ceiling */
    unsigned __int128 t = (unsigned __int128)m * (m + 1) / 2;
    if (t < n)
        ++m;

    return (seq_t)(n % m);
}
