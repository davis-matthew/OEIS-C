/*
 * A001361  Number of points in interior of n-th crystal ball in E_8 lattice.
 * https://oeis.org/A001361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..8} c_k * binomial(n+7-k, 8), with c = [1, 232,
 *            24508, 107224, 133510, 55384, 7228, 232, 1] (from the G.f.
 *            x*P(x)/(1-x)^9)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 197, u128 n <= 1000,
 *            big any n
 * Helpers:   a001361_binom8, a001361_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001361_binom8.h"
#include "../utils/a001361_gcd.h"

seq_t a(idx_t n) {
    static const uint32_t c[9] = {1, 232, 24508, 107224, 133510, 55384, 7228, 232, 1};
    seq_t result = 0;
    int k;
    for (k = 0; k <= 8; k++) {
        uint64_t m = (uint64_t)n + 7;
        if (m < (uint64_t)k) continue;
        m -= (uint64_t)k;
        result += (seq_t)c[k] * a001361_binom8(m);
    }
    return result;
}
