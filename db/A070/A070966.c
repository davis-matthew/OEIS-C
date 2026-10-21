/*
 * A070966  a(n) = Sum_{k|n, k<=sqrt(n)} phi(k); where the sum is over the
 *          positive divisors, k, of n, which are <= the square root of n; and
 *          phi(k) is the Euler totient function.
 * https://oeis.org/A070966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k|n, k<=sqrt(n)} phi(k)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"

/* Floor integer square root (only idx_t inputs) */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { lo = 1; continue; }            /* avoid div by zero      */
        if (mid > n / mid)              /* mid*mid > n  (overflow‑safe) */
            hi = mid - 1;
        else                            /* mid*mid <= n                      */
        {
            if (mid + 1 > n / (mid + 1))
                return mid;
            lo = mid + 1;
        }
    }
    return hi;   /* never reached, but keeps compiler happy */
}

/* External function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* defined only for n >= 1 */

    idx_t limit = isqrt_u64(n);
    seq_t sum = 0;
    for (idx_t k = 1; k <= limit; ++k)
    {
        if (n % k == 0)
            sum += calc_phi((seq_t)k);
    }
    return sum;
}
