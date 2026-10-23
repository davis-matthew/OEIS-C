/*
 * A117004  a(n) = sigma(n) - A079667(n).
 * https://oeis.org/A117004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} min(d, n/d)
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root (floor) for 64‑bit indices. */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 32);   /* 2^32 – exclusive upper bound */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = mid + 1; continue; }
        if (mid <= n / mid)        /* mid^2 <= n  */
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;               /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t r = int_sqrt_u64(n);
    seq_t sum = 0;
    for (idx_t d = 1; d <= r; ++d)
    {
        if (n % d == 0)
        {
            sum += (seq_t)d;
            idx_t q = n / d;
            if (q != d)
                sum += (seq_t)d;
        }
    }
    return sum;
}
