/*
 * A096895  a(n) = A088557(n)/4.
 * https://oeis.org/A096895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A088557(n)/4 = min{ m*k/2 : (m-k)(m+k) = 2n+1,
 *            gcd(m,k)=1, m>k>0 }; computed by divisor search
 * Range:     u8 n <= 22, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t L = (seq_t)2 * (seq_t)n + (seq_t)1;
    seq_t best = 0;
    for (seq_t u = 1; u * u < L; u += 2) {
        if (L % u != 0) continue;
        seq_t v = L / u;
        if (gcd(u, v) != 1) continue;
        seq_t m = (u + v) / 2;
        seq_t k = (v - u) / 2;
        seq_t val = (m * k) / 2;
        if (best == 0 || val < best) best = val;
    }
    return best;
}
