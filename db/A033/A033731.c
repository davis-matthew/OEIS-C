/*
 * A033731  Expansion of Product_{d | 19} theta_3(q^d).
 * https://oeis.org/A033731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a,b≥0, a² + 19·b² = n} (a==0?1:2)·(b==0?1:2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* handle a = 0 separately (avoid unnecessary loop) */
    {
        idx_t m = n;                      /* k = 0 */
        if (m % 19 == 0) {
            idx_t t = m / 19;            /* b² */
            idx_t r = _floor_isqrt_u64(t);
            if (r * r == t) {
                seq_t w1 = 1;             /* a == 0 */
                seq_t w2 = (t == 0) ? 1 : 2;
                sum += w1 * w2;
            }
        }
    }

    /* iterate over positive a (a ≥ 1) */
    idx_t lim = _floor_isqrt_u64(n);
    for (idx_t a = 1; a <= lim; ++a) {
        idx_t k = a * a;                 /* a² */
        idx_t m = n - k;
        if (m % 19 == 0) {
            idx_t t = m / 19;            /* b² */
            idx_t r = _floor_isqrt_u64(t);
            if (r * r == t) {
                seq_t w1 = 2;             /* a > 0 */
                seq_t w2 = (t == 0) ? 1 : 2;
                sum += w1 * w2;
            }
        }
    }

    return sum;
}
