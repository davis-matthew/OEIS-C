/*
 * A204560  Symmetric matrix: f(i,j)=floor[(2i+2j+4)/4]-floor[(i+j+2)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor( ceil((1+sqrt(1+8n))/2) / 4 ) + 1 (s is the
 *            smallest integer with s*(s-1)/2 >= n, a(n) = floor(s/4)+1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer floor square root for 64‑bit unsigned values */
static idx_t uint64_floor_sqrt(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1ULL << 32;   /* > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* OEIS A204560 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, safe fallback */

    /* solve s(s-1)/2 >= n */
    idx_t D = 1 + 8 * n;                 /* 1+8n fits in 64 bits for used n */
    idx_t t = uint64_floor_sqrt(D);      /* floor(sqrt(D)) */
    idx_t s = (t + 1) >> 1;              /* initial guess (t+1)/2 */

    /* adjust, at most a few increments */
    while (((unsigned __int128)s * (s - 1)) / 2 < n)
        ++s;

    /* value on that diagonal: floor(s/4)+1 */
    return (seq_t)(s / 4 + 1);
}
