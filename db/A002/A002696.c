/*
 * A002696  Binomial coefficients C(2n,n-3).
 * https://oeis.org/A002696
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-3), computed multiplicatively: C(2n,i+1) =
 *            C(2n,i)*(2n-i)/(i+1), dividing out gcd first to avoid overflow
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   a002696_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002696_g.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t k = n - 3;
    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        uint64_t den = (uint64_t)(i + 1);
        uint64_t num = (uint64_t)(2 * n - i);
        uint64_t r = (uint64_t)(result % (seq_t)den);
        uint64_t g = a002696_g(den, r);   /* gcd(result, den) */
        result /= (seq_t)g;
        den /= g;                          /* den now divides num */
        num /= den;
        result *= (seq_t)num;
    }
    return result;
}
