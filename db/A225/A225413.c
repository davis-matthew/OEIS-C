/*
 * A225413  Triangle read by rows: T(n,k) = (A101164(n,k) - A014473(n,k))/2.
 * https://oeis.org/A225413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,k) = (A008288(n,k) - 2*A007318(n,k) + 1)/2 =
 *            Sum_{j=2..min(k,n-k)} C(n-k,j)*C(k,j)*(2^(j-1)-1) (using the
 *            Delannoy sum D = Sum_j C(n-k,j)C(k,j)2^j and the Vandermonde
 *            identity C(n,k) = Sum_j C(n-k,j)C(k,j); every term is >= 0, so
 *            no intermediate value exceeds the result). a(m) = T(r,k), m =
 *            r...
 * Range:     u8 n <= 58, u16 n <= 142, u32 n <= 446, u64 n <= 1509,
 *            u128 n <= 5150, big any n
 * Validated: standard, pass, 94 terms through n = 5006 (2026-11-07)
 * Helpers:   binomial, isqrt_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r > 0 && r * (r + 1) / 2 > n) --r;
    idx_t k = n - r * (r + 1) / 2;

    idx_t p = r - k;
    idx_t m = (p < k) ? p : k;
    seq_t res = 0;
    for (idx_t j = 2; j <= m; ++j) {
        seq_t w = pow2(j - 1);
        w -= 1;
        seq_t term = binomial(p, j);
        term *= binomial(k, j);
        term *= w;
        res += term;
    }
    return res;
}
