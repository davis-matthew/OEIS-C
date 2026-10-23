/*
 * A119901  Difference between two consecutive squares enclosing 3^(2n+1).
 * https://oeis.org/A119901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Floor(3^((2n+1)/2)) + 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   floor_sqrt, mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/floor_sqrt.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Small‐integer path (seq_t <= 64 bits).  Use __uint128_t
     * to avoid overflow when computing 3^n and the required
     * integer square root. */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
    {
        unsigned __int128 x = 1;          /* x = 3^n       */
        for (idx_t i = 0; i < n; ++i)
            x *= 3u;

        unsigned __int128 y = x * x * 3u; /* y = 3 * x^2   */

        /* Integer square root of y using binary search.  */
        unsigned __int128 low  = 0;
        unsigned __int128 high = y + 1;          /* sqrt(y) < high */
        while (low + 1 < high)
        {
            unsigned __int128 mid = (low + high) >> 1;
            if (mid * mid <= y)
                low = mid;
            else
                high = mid;
        }

        unsigned __int128 m = low * 2 + 1;        /* 2*floor + 1  */
        return (seq_t)m;
    }

    /* Large integer (big integer) path.  For seq_t that is a big‑int
     * class, pow_seq, floor_sqrt, and mul_small work properly. */
    idx_t exp = (n << 1) + 1;   /* 2n + 1 */
    seq_t p   = pow_seq((seq_t)3, exp);
    seq_t s   = floor_sqrt(p);
    seq_t res = mul_small(s, 2);
    res += (seq_t)1;
    return res;
}
