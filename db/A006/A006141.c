/*
 * A006141  Number of integer partitions of n whose smallest part is equal to
 *          the number of parts.
 * https://oeis.org/A006141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Sum_{m>=1} x^(m^2)/Product_{k=1..m-1} (1-x^k), i.e. a(n) =
 *            Sum_{m>=1, m^2<=n} (number of partitions of n-m^2 into parts <=
 *            m-1)
 * Range:     u8 n <= 45, u16 n <= 112, u32 n <= 320, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t len = n + 1;
    seq_t *c = alloc_zero(len); /* c[j] = partitions of j into parts <= m-1 */
    seq_t result = 0;
    idx_t m, j;
    c[0] = 1;
    for (m = 1; m * m <= n; m++) {
        result += c[n - m * m];
        /* include parts of size m for the next round */
        for (j = m; j <= n; j++)
            c[j] += c[j - m];
    }
    release(c, len);
    return result;
}
