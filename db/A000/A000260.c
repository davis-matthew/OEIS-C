/*
 * A000260  Number of rooted simplicial 3-polytopes with n+3 nodes; or rooted
 *          3-connected triangulations with 2n+2 faces; or rooted 3-connected
 *          trivalent maps with 2n+2 vertices.
 * https://oeis.org/A000260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 8*(4*n+1)*(2*n-1)*(4*n-1)*a(n-1) /
 *            (3*(3*n+2)*(3*n+1)*(n+1)), a(0) = 1 (equivalently a(n) =
 *            2*(4*n+1)! / ((n+1)!*(3*n+2)!))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 186 (2026-11-01)
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
    for (k = 1; k <= n; k++) {
        unsigned long long num = 8ULL * (4 * k + 1) * (2 * k - 1) * (4 * k - 1);
        unsigned long long den = 3ULL * (3 * k + 2) * (3 * k + 1) * (k + 1);
        /* Cancel den against num and v so the product num*v equals the next
           term exactly, with nothing larger than the result. */
        while (den > 1) {
            unsigned long long g = gcd_u64(num, den);
            if (g > 1) {
                num /= g;
                den /= g;
                continue;
            }
            g = gcd_u64((unsigned long long)(v % (seq_t)den), den);
            if (g <= 1)
                break; /* only reachable after fixed-width overflow */
            v /= (seq_t)g;
            den /= g;
        }
        v *= (seq_t)num;
    }
    return v;
}
