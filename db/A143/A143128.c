/*
 * A143128  a(n) = Sum_{k=1..n} k*sigma(k).
 * https://oeis.org/A143128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k * σ(k) * where σ(k) is the sum of all
 *            divisors of k
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 1985, u64 n <= 4267,
 *            u128 n <= 4267, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C includes – sufficient for both C11 and C++17 */
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------- */
/* Integer square‑root for 64‑bit unsigned values, using binary search. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        /* use 128‑bit to avoid overflow when mid*mid > 2^64-1 */
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)          return mid;
        if (sq < n)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;   /* floor(sqrt(n)) */
}

/* ------------------------------------------------------------------- */
/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                /* sequence is defined for n ≥ 1, but 0 is harmless */
        return (seq_t)0;

    seq_t total = 0;           /* will hold the final sum */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute σ(k) by summing divisors up to sqrt(k)   */
        seq_t sigma = 0;
        idx_t r = isqrt_u64(k);

        for (idx_t d = 1; d <= r; ++d)
        {
            if (k % d == 0)
            {
                sigma += (seq_t)d;
                idx_t q = k / d;
                if (q != d)
                    sigma += (seq_t)q;
            }
        }

        /* accumulate k * σ(k) into the running total */
        total += (seq_t)k * sigma;
    }

    return total;
}
