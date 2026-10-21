/*
 * A232179  Least k >= 0 such that n^2 + triangular(k) is a triangular number.
 * https://oeis.org/A232179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k ≥ 0 such that n^2 + k(k+1)/2 is triangular
 * Range:     u8 n <= 16, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    idx_t ms;
    while (lo <= hi)
    {
        ms = lo + ((hi - lo) >> 1);
        if (ms * ms == n)
            return ms;
        if (ms * ms < n)
            lo = ms + 1;
        else
            hi = ms - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return 0;

    /* M = 2 * n^2 (use seq_t for intermediate to avoid overflow) */
    seq_t sq = (seq_t)n * (seq_t)n;
    idx_t M = (idx_t)(sq * 2);

    idx_t min_k = ~((idx_t)0);          /* maximum value */

    idx_t limit = isqrt_u64(M);
    for (idx_t d = 1; d <= limit; ++d)
    {
        if (M % d != 0)
            continue;

        idx_t d1 = d;          /* s - t */
        idx_t d2 = M / d;      /* s + t + 1 */

        /* Compute t = (d2 - d1 - 1) / 2; must be integer and >= 0 */
        idx_t diff = d2 - d1 - 1;
        if (diff >= 0 && (diff & 1) == 0)
        {
            idx_t k = diff >> 1;
            if (k < min_k)
                min_k = k;
        }
    }

    /* For n > 0 a solution always exists; return the minimal k found */
    return (seq_t)min_k;
}
