/*
 * A211511  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2=2x*y.
 * https://oeis.org/A211511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of ordered triples (w,x,y) with 1 <= w,x,y <= n and w^2
 *            = 2xy
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    if (n < 2) return 0;

    idx_t maxK = n / 2;
    for (idx_t k = 1; k <= maxK; ++k) {
        seq_t N = (seq_t)2 * (seq_t)k * (seq_t)k;          /* N = 2k^2 */
        idx_t sqrtN = (idx_t)floor_sqrt(N);               /* floor(sqrt(N)) */
        idx_t limitX = (sqrtN < n) ? sqrtN : n;           /* iterate x <= min(n, sqrtN) */

        for (idx_t x = 1; x <= limitX; ++x) {
            if ((N % x) == 0) {
                seq_t y = N / x;
                if (y <= n) {
                    if (x == y) total += 1;               /* pair (x,x) counted once */
                    else       total += 2;               /* ordered pair (x,y) and (y,x) */
                }
            }
        }
    }
    return total;
}
