/*
 * A088226  a(1)=0, a(2)=0, a(3)=1; for n>3, a(n) = abs(a(n-1) - a(n-2) -
 *          a(n-3)).
 * https://oeis.org/A088226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(k^2 + 2*m + 2) = k - m a(k^2 + 2*m + 1) = m for k >= 0, 0 <= m
 *            <= k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor of the square root of an unsigned 64‑bit integer */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 0xFFFFFFFFULL;      /* (2^32‑1)^2 < 2^64 */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low - 1;                   /* floor[sqrt(n)] */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* safety, offset is 1 */

    /* find k such that k^2 < n <= k^2 + 2k + 2   (k = floor(sqrt(n‑1))) */
    idx_t k = isqrt_u64(n - 1);

    /* d = n - k^2  (1 <= d <= 2k+2) */
    idx_t d = n - k * k;

    if (d & 1U) {                     /* odd: n = k^2 + 2m + 1  -> a = m = (d‑1)/2 */
        return (seq_t)((d - 1) / 2);
    } else {                           /* even: n = k^2 + 2m + 2 -> a = k - m = k - d/2 + 1 */
        return (seq_t)(k - (d >> 1) + 1);
    }
}
