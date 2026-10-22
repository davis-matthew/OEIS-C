/*
 * A006704  Solution to Pellian: x such that x^2 - n y^2 = +- 1, +- 4.
 * https://oeis.org/A006704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a square; otherwise the least x > 0 with x^2 -
 *            n*y^2 in {+1,-1,+4,-4}, y >= 1. For n <= 16 this is found by a
 *            direct search. For n > 16 every coprime solution with |N| <= 4 <
 *            sqrt(n) is a convergent p_k/q_k of the continued fraction of
 *            sqrt(n). Since p_k^2 - n*q_k^2 = (-1)^(k+1)*...
 * Range:     u8 n <= 30, u16 n <= 93, u32 n <= 198, u64 n <= 525,
 *            u128 n <= 1725, big any n
 * Helpers:   is_sq_u64, isqrt_u64_loc, pell_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_sq_u64.h"
#include "../utils/isqrt_u64_loc.h"
#include "../utils/pell_ok.h"

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t a0 = isqrt_u64_loc(nn);
    if (a0 * a0 == nn) return (seq_t)1;

    if (nn <= 16) {
        uint64_t x;
        for (x = 1; ; x++) {
            uint64_t x2 = x * x;
            if (pell_ok(x2 + 1, nn) || pell_ok(x2 + 4, nn) ||
                (x2 >= 1 && pell_ok(x2 - 1, nn)) ||
                (x2 >= 4 && pell_ok(x2 - 4, nn)))
                return (seq_t)x;
        }
    }

    {
        seq_t pm1 = 1, pm2 = 0;
        uint64_t m = 0, d = 1, aa = a0;
        for (;;) {
            seq_t p = (seq_t)aa * pm1 + pm2;
            m = d * aa - m;
            d = (nn - m * m) / d;
            if (d == 1 || d == 4) return p;
            aa = (a0 + m) / d;
            pm2 = pm1;
            pm1 = p;
        }
    }
}
