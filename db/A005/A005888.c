/*
 * A005888  Theta series of hexagonal close-packing with respect to edge
 *          between layers.
 * https://oeis.org/A005888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{(a,b,m) in Z^3 : 2(2a+1)^2 + 3(2b+1)^2 + 4(3m+1)^2 = n}
 *            + #{(a,b,m) in Z^3 : 2(2a+1)^2 + 12b^2 + (6m+1)^2 = n}, i.e. the
 *            coefficient of q^(n/6) in theta2(q^(4/3)) *
 *            (theta2(q^2)*psi(3,q^6) + theta3(q^2)*psi(6,q^6))
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t cnt = 0;
    uint64_t u, v, b;

    /* First part: u = |2a+1| odd (x2), v = |2b+1| odd (x2), t = 3m+1 */
    for (u = 1; 2 * u * u <= N; u += 2) {
        for (v = 1; 2 * u * u + 3 * v * v <= N; v += 2) {
            uint64_t r = N - 2 * u * u - 3 * v * v;
            if (r % 4 != 0) continue;
            r /= 4;
            uint64_t s = a000037_isqrt(r);
            if (s * s != r) continue;
            /* t = s if s%3==1, t = -s if s%3==2; s%3==0 gives no t = 1 mod 3 */
            if (s % 3 != 0) cnt += 4;
        }
    }

    /* Second part: u = |2a+1| odd (x2), b any integer, y = 6m+1 */
    for (u = 1; 2 * u * u <= N; u += 2) {
        for (b = 0; 2 * u * u + 12 * b * b <= N; b++) {
            uint64_t r = N - 2 * u * u - 12 * b * b;
            uint64_t s = a000037_isqrt(r);
            if (s * s != r) continue;
            if (s % 6 == 1 || s % 6 == 5)
                cnt += (b == 0) ? 2 : 4;
        }
    }

    return (seq_t)cnt;
}
