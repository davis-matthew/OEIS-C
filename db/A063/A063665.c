/*
 * A063665  Number of ways 1/n can be written as 1/x^2 + 1/y^2 with y >= x >=
 *          1.
 * https://oeis.org/A063665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8787, u32 n <= 9651, u64 n <= 9651,
 *            u128 n <= 9651, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063665:
 * a(n) = number of ordered pairs (x,y) with y >= x >= 1 satisfying
 * 1/n = 1/x^2 + 1/y^2.  Equivalently n = x^2 y^2 / (x^2 + y^2). */

#include "../utils/floor_sqrt.h"

static inline idx_t isqrt_idx(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 0;
    idx_t hi = (n >> 1) + 1;   /* hi >= sqrt(n) */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)  /* mid^2 <= n  */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;          /* offset starts at 1, but guard anyway */

    idx_t x_start = isqrt_idx(n) + 1;          /* smallest integer > sqrt(n) */
    idx_t x_end   = isqrt_idx((idx_t)((unsigned __int128)2 * n)); /* floor(sqrt(2n)) */

    for (idx_t x = x_start; x <= x_end; ++x)
    {
        seq_t xsq = (seq_t)x * x;          /* x^2 */
        seq_t d   = xsq - (seq_t)n;        /* x^2 - n > 0 */

        seq_t num = (seq_t)n * xsq;        /* n * x^2 */

        if (num % d != 0) continue;        /* y^2 not integer */

        seq_t y2 = num / d;                /* y^2 */
        seq_t y  = floor_sqrt(y2);          /* integer sqrt */

        if (y * y != y2) continue;         /* y^2 not a perfect square */
        if (y < (seq_t)x) continue;        /* enforce y >= x */

        count++;                           /* found a valid pair (x,y) */
    }
    return count;
}
