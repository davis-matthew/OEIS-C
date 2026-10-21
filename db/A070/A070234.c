/*
 * A070234  Let u(k), v(k), w(k) satisfy the recursions u(1) = v(1) = w(1) =
 *          1, u(k+1) = u(k) + v(k) + w(k), v(k+1) = u(k)*v(k) + v(k)*w(k) +
 *          w(k)*u(k), and w(k+1) = u(k)*v(k)*w(k); then a(n) = v(n).
 * https://oeis.org/A070234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is defined by u(1)=v(1)=w(1)=1, u(k+1)=u(k)+v(k)+w(k),
 *            v(k+1)=u(k)*v(k)+v(k)*w(k)+w(k)*u(k), w(k+1)=u(k)*v(k)*w(k);
 *            a(n) = v(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t u = (seq_t)1, v = (seq_t)1, w = (seq_t)1;
    idx_t k;
    for (k = 1; k < n; ++k)
    {
        seq_t next_u = u + v + w;                  // u(k+1)
        seq_t next_v = u * v + v * w + w * u;      // v(k+1)
        seq_t next_w = u * v * w;                 // w(k+1)
        u = next_u;
        v = next_v;
        w = next_w;
    }
    return v;
}
