/*
 * A137735  a(0)=1. a(n) = floor(n/b(n)), where b(n) is the largest value
 *          among (a(0),a(1),...,a(n-1)).
 * https://oeis.org/A137735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=floor(n / b(n)), where b(n)=max_{i<n}a(i). Equivalent
 *            closed form (derived in the OEIS entry): Let m=floor(sqrt(n)).
 *            Then a(0)=1, a(n)=m+1 if n=m*m+m, otherwise a(n)=m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for 0≤n≤UINT64_MAX. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t m      = isqrt_idx(n);
    unsigned __int128 sq = (unsigned __int128)m * m;

    if ((unsigned __int128)n == sq + m)
        return (seq_t)(m + 1);
    else
        return (seq_t)m;
}
