/*
 * A056942  Area of rectangle needed to enclose a non-touching spiral of
 *          length n on a square lattice.
 * https://oeis.org/A056942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = floor((√(8n+1)-1)/2) * ceil((√(8n+1)-1)/2)
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the requested term of OEIS A056942.
   It uses only integer arithmetic; no floating point or external libraries. */
#include <stdint.h>

/* Integer t solving t*(t+1)/2 <= n < (t+1)*(t+2)/2  */
static inline idx_t triangular_root(idx_t n)
{
    idx_t lo = 0, hi = n + 1;          /* hi is exclusive upper bound */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* mid*(mid+1)/2 fits in unsigned __int128 */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find the largest t such that t*(t+1)/2 <= n */
    idx_t t = triangular_root(n);

    /* Compute triangular number t*(t+1)/2 precisely */
    idx_t tri = (unsigned __int128)t * (t + 1) / 2;

    /* Determine ceil part: t or t+1 */
    idx_t tceil = (n == tri) ? t : t + 1;

    /* Product fits into seq_t (the build‑specific integer type) */
    return (seq_t)t * (seq_t)tceil;
}
