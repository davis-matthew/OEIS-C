/*
 * A174462  a(n) = Sum_{d|n} binomial(n,d)^2.
 * https://oeis.org/A174462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(n,d)^2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* a(0) is undefined, return 0 for safety */

    seq_t total = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t v = binomial(n, d);
            total += v * v;            /* add binom(n,d)^2 */

            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t w = binomial(n, d2);
                total += w * w;        /* add binom(n,n/d)^2 */
            }
        }
    }

    return total;
}
