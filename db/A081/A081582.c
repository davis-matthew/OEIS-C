/*
 * A081582  Pascal-(1,7,1) array.
 * https://oeis.org/A081582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(d,k), where * d = floor((sqrt(8*n+1)-1)/2) * k = n -
 *            d*(d+1)/2 * * T(d,k) is the entry of the Pascal-(1,7,1) array
 *            and is given by * T(d,k) = sum_{j=0}^{d-k} C(d-k,j) * C(k,j) *
 *            8^j * where C(n,j) is binomial(n,j). The array is enumerated by
 *            * antidiagonals: all pairs (k,m) with k+m = d...
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 178, u64 n <= 645,
 *            u128 n <= 2377, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

static inline idx_t triangular(idx_t t) { return t * (t + 1) / 2; }

seq_t a(idx_t n) {
    /* Determine the antidiagonal index d and offset k */
    long double root = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t d = (idx_t)((root - 1.0L) / 2.0L);
    /* Correct possible rounding errors */
    while (triangular(d) > n) --d;
    while (triangular(d + 1) <= n) ++d;
    idx_t k = n - triangular(d);

    /* Compute T(d,k) = sum_{j=0}^{d-k} binomial(d-k,j)*binomial(k,j)*8^j */
    seq_t sum = 0;
    seq_t pow8 = 1;               /* 8^0 */
    idx_t maxj = d - k;
    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t t = binomial(d - k, j);
        t *= binomial(k, j);
        t *= pow8;
        sum += t;
        pow8 *= (seq_t)8;
    }

    return sum;
}
