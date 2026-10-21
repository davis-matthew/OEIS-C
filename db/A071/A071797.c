/*
 * A071797  Restart counting after each new odd integer (a fractal sequence).
 * https://oeis.org/A071797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = n - floor(sqrt(n-1))^2 (for n ≥ 1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the “restarting count after each new odd integer” sequence. */

#include <stdint.h>   /* for uint64_t used by the build as idx_t */

static idx_t isqrt_u64(idx_t x)
{
    /* Compute ⌊sqrt(x)⌋ using integer arithmetic (binary search). */
    idx_t low = 0, high = x, mid, best = 0;
    while (low <= high)
    {
        mid = low + ((high - low) >> 1);
        if (mid == 0)
        {
            best = 0;
            low = 1;
            continue;
        }
        if (mid <= x / mid)
        {
            best = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* Return 0 when n == 0 (though offset is 1); for n ≥ 1 use the formula. */
    if (n == 0)
        return (seq_t)0;

    idx_t r = isqrt_u64(n - 1);      /* floor(sqrt(n-1)) */
    idx_t sq = r * r;                /* r^2 fits in idx_t (uint64_t) */
    return (seq_t)n - (seq_t)sq;     /* a(n) = n - r^2 */
}
