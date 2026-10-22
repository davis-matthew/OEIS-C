/*
 * A000743  Number of compositions of n into 5 ordered relatively prime parts.
 * https://oeis.org/A000743
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * binomial(n/d - 1, 4) (Moebius transform
 *            of binomial(n-1,4))
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 570, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a743_binom4, a743_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a743_binom4.h"
#include "../utils/a743_mu.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            idx_t q = n / d;
            int mu;
            if (q >= 5) {
                mu = a743_mu((uint64_t)d);
                if (mu > 0) result += a743_binom4((uint64_t)(q - 1));
                else if (mu < 0) result -= a743_binom4((uint64_t)(q - 1));
            }
            if (d != q && d >= 5) {
                mu = a743_mu((uint64_t)q);
                if (mu > 0) result += a743_binom4((uint64_t)(d - 1));
                else if (mu < 0) result -= a743_binom4((uint64_t)(d - 1));
            }
        }
    }
    return result;
}
