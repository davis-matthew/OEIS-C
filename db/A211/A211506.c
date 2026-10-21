/*
 * A211506  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2=x*y+n.
 * https://oeis.org/A211506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (w,x,y) with 1 <= w,x,y <= n and w^2 =
 *            x*y + n
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   uint128_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/uint128_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;   /* early exit for n = 0 */

    for (idx_t w = 1; w <= n; ++w) {
        unsigned __int128 w2 = (unsigned __int128)w * w;
        if (w2 < n) continue;                     /* w^2 - n would be negative */
        unsigned __int128 M = w2 - n;              /* M = w^2 - n >= 1 */

        uint64_t limit = uint128_sqrt(M);
        if (limit > n) limit = n;                  /* we only need divisors <= n */

        for (uint64_t d = 1; d <= limit; ++d) {
            if (M % d != 0) continue;              /* not a divisor */
            unsigned __int128 q = M / d;
            if (q > n) continue;                   /* y would be out of range */

            if (d == (uint64_t)q)                  /* square divisor */
                count += (seq_t)1;
            else
                count += (seq_t)2;                  /* unordered pair gives two ordered pairs */
        }
    }
    return count;
}
