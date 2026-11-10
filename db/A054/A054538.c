/*
 * A054538  A000013 / 2.
 * https://oeis.org/A054538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000013(n+1)/2 = (1/(4*(n+1))) * Sum_{d|(n+1)}
 *            phi(2*d)*2^((n+1)/d), where A000013(m) = Sum_{d|m}
 *            phi(2d)*2^(m/d)/(2m)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

seq_t a(idx_t n)
{
    uint64_t m, N, d, rsum;
    seq_t total;

    m = (uint64_t)n + 1;
    N = 4 * m;
    total = 0;
    rsum = 0;

    for (d = 1; d <= m; d++) {
        uint64_t c, k, i, r;
        seq_t q;
        if (m % d != 0)
            continue;
        c = totient_u64(2 * d);
        k = m / d;
        q = (seq_t)(c / N);
        r = c % N;
        for (i = 0; i < k; i++) {
            q += q;
            r <<= 1;
            if (r >= N) {
                r -= N;
                q += 1;
            }
        }
        total += q;
        rsum += r;
    }
    total += (seq_t)(rsum / N);
    return total;
}
