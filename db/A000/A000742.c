/*
 * A000742  Number of compositions of n into 4 ordered relatively prime parts.
 * https://oeis.org/A000742
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * C(n/d - 1, 3) (Moebius transform of
 *            C(n-1,3))
 * Range:     u8 n <= 13, u16 n <= 76, u32 n <= 2956, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a742_add, a742_c3, a742_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a742_add.h"
#include "../utils/a742_c3.h"
#include "../utils/a742_mu.h"

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            a742_add(&pos, &neg, n, d);
            if (n / d != d) a742_add(&pos, &neg, n, n / d);
        }
    }
    return pos - neg;
}
