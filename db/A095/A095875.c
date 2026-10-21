/*
 * A095875  Number of lattice points on graph of parabola y >= x^2 with y <=
 *          n.
 * https://oeis.org/A095875
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} (2*⌊√k⌋ + 1) (a(-1)=0)
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;           /* a(-1) = 0 */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t m = floor_sqrt((seq_t)k);   /* m = ⌊√k⌋ */
        sum += (seq_t)1 + (seq_t)2 * m;   /* add contribution of y = k */
    }
    return sum;
}
