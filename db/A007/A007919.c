/*
 * A007919  Smallest k such that k*n is a double factorial.
 * https://oeis.org/A007919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m!!/n where m is the least m >= 1 with n | m!! (found by
 *            a search: divide n by gcd with each factor of the odd and even
 *            chains)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 22, u64 n <= 36, u128 n <= 58,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 805 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    uint64_t r[2];
    uint64_t m;
    r[0] = (uint64_t)n;
    r[1] = (uint64_t)n;
    for (m = 1;; m++) {
        uint64_t *rp = &r[m & 1];
        *rp /= gcd_u64(*rp, m);
        if (*rp == 1) break;
    }
    /* compute m!! / n exactly, cancelling n factor by factor */
    {
        seq_t res = 1;
        uint64_t d = (uint64_t)n;
        uint64_t f;
        for (f = m; f >= 1; f -= 2) {
            uint64_t g = gcd_u64(f, d);
            d /= g;
            res *= (seq_t)(f / g);
            if (f < 2) break;
        }
        return res;
    }
}
