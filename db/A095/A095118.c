/*
 * A095118  a(n) is the sum of the squares of the divisors of n which are <=
 *          sqrt(n).
 * https://oeis.org/A095118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d | n , d ≤ √n} d²
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate over all integers d from 1 to ⌊√n⌋, add d²
   to the sum when d divides n.   */
#include <stdbool.h>   /* for bool, if needed elsewhere */

/* Integer square‑root for unsigned 64‑bit values (returns the floor of √n). */
static inline idx_t my_isqrt(idx_t n)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32) - 1; /* √(2^64−1) < 2^32 */
    idx_t r = 0;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > n / mid)          /* mid² > n */
            hi = mid - 1;
        else
        {
            r = mid;
            lo = mid + 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t sq = my_isqrt(n);          /* floor(sqrt(n)) */
    for (idx_t d = 1; d <= sq; ++d)
    {
        if (n % d == 0)              /* d divides n */
        {
            seq_t dsq = (seq_t)d * (seq_t)d;
            sum += dsq;
        }
    }
    return sum;
}
