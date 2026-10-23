/*
 * A036707  a(n)=number of Gaussian integers z=a+bi satisfying |z|<=n+1/2,
 *          b>=0.
 * https://oeis.org/A036707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{b=0}^{n} (2 * floor_sqrt(n^2 + n - b^2) + 1)
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* counts the Gaussian integers a+bi with |a+bi| ≤ n+1/2 and b ≥ 0 */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t res = 0;
    for (idx_t b = 0; b <= n; ++b) {
        seq_t bb  = (seq_t)b * (seq_t)b;                 /* b^2 */
        seq_t sq  = N * N + N - bb;                      /* n^2 + n - b^2 */
        seq_t ma  = floor_sqrt(sq);                      /* floor sqrt */
        res += (ma << 1) + 1;                            /* 2*ma + 1 */
    }
    return res;
}
