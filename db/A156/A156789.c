/*
 * A156789  Irregular triangle, read by rows, T(n, k) = binomial(2*n,
 *          k)*binomial(2*k, k).
 * https://oeis.org/A156789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(2*floor(sqrt(n)), k) * binomial(2*k, k) * where
 *            r = floor(sqrt(n)) and k = n - r*r
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 75, u64 n <= 248, u128 n <= 886,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    idx_t r = _floor_isqrt_u64(n);
    idx_t k = n - r * r;
    seq_t v1 = binomial((idx_t)(2 * r), k);
    seq_t v2 = binomial((idx_t)(2 * k), k);
    return v1 * v2;
}
