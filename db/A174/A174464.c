/*
 * A174464  a(n) = Sum_{d|n} C(n,d)^(n/d).
 * https://oeis.org/A174464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(n,d)^(n/d)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but handle 0 safely */

    seq_t sum = (seq_t)0;
    idx_t d;

    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t bin = binomial(n, d);
            seq_t exp = (seq_t)(n / d);
            sum += big_pow(bin, exp);

            /* paired divisor k = n/d, if distinct */
            idx_t k = n / d;
            if (k != d) {
                seq_t bink = binomial(n, k);
                seq_t expk = (seq_t)(n / k);   /* which is d */
                sum += big_pow(bink, expk);
            }
        }
    }
    return sum;
}
