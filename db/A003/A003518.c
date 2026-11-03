/*
 * A003518  a(n) = 8*binomial(2*n+1,n-3)/(n+5).
 * https://oeis.org/A003518
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 2*(2n+1)*n / ((n-3)*(n+5)), a(3) = 1 (equals
 *            8*binomial(2n+1,n-3)/(n+5))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 492 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n)
{
    seq_t v = 1;
    idx_t k;
    for (k = 3; k < n; k++) {
        uint64_t N = 2 * (2 * (uint64_t)k + 3) * ((uint64_t)k + 1);
        uint64_t D = ((uint64_t)k - 2) * ((uint64_t)k + 6);
        seq_t m = (seq_t)D;
        uint64_t r, g;
        seq_t gs;
        if (m == 0) break; /* width overflow: value no longer representable */
        r = (uint64_t)(v % m);
        g = gcd_u64(D, r);
        gs = (seq_t)g;
        if (gs == 0) break;
        v /= gs;
        D /= g;
        v *= (seq_t)(N / D);
    }
    return v;
}
