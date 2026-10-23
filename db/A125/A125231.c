/*
 * A125231  Triangle read by rows: T(n,k) = ceiling((k+1)/2)*binomial(n,k) (0
 *          <= k <= n).
 * https://oeis.org/A125231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle read by rows * T(r,k) =
 *            ceil((k+1)/2) * binomial(r,k), * where r is the row number
 *            (starting at 0) and k is the column * within that row (0 ≤ k ≤
 *            r). The triangle is flattened left‑to‑right
 * Range:     u8 n <= 39, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* integer square root for 64‑bit unsigned integers */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 1ULL << 32;      /* hi > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t t   = n * (idx_t)8 + 1;            /* 8*n + 1 */
    idx_t s   = isqrt_u64(t);
    idx_t r   = (s - 1) / 2;
    idx_t base = r * (r + 1) / 2;
    while (base > n) {
        r   -= 1;
        base = r * (r + 1) / 2;
    }
    while ((r + 1) * (r + 2) / 2 <= n) {
        r   += 1;
        base = r * (r + 1) / 2;
    }

    idx_t k = n - base;                     /* column in the row */
    idx_t mult = (k + 2) / 2;               /* ceil((k+1)/2) */

    seq_t val = binomial(r, k);             /* binomial coefficient */
    val *= (seq_t)mult;                     /* multiply by the factor */

    return val;
}
