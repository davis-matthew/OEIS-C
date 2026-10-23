/*
 * A138764  E.g.f. A(x) equals the inverse function of log(x)/(x + x^2).
 * https://oeis.org/A138764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 0..n} binomial(n, k) * (n + k + 1)^(n-1) (for n
 *            = 0, a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t res = 0;
    seq_t exp = (seq_t)(n - 1);
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n, k);
        seq_t base = (seq_t)(n + k + 1);
        seq_t p = big_pow(base, exp);
        res += bin * p;
    }
    return res;
}
