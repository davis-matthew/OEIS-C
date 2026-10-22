/*
 * A000741  Number of compositions of n into 3 ordered relatively prime parts.
 * https://oeis.org/A000741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * binomial(d-1, 2) (Moebius transform
 *            of A000217(n-2))
 * Range:     u8 n <= 24, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a741_add, a741_c2, a741_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a741_add.h"
#include "../utils/a741_c2.h"
#include "../utils/a741_mu.h"

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    idx_t d;
    if (n < 3) return 0;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            idx_t q = n / d;
            a741_add(n, d, &pos, &neg);
            if (q != d) a741_add(n, q, &pos, &neg);
        }
    }
    return pos - neg;
}
