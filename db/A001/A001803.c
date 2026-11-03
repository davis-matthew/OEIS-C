/*
 * A001803  Numerators in expansion of (1 - x)^(-3/2).
 * https://oeis.org/A001803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (2*n+1)!!/oddpart(n!), i.e. a(n) =
 *            a(n-1)*(2*n+1)/oddpart(n), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 200 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t o = (uint64_t)k;
        uint64_t m, g, q, t;
        while ((o & 1) == 0) o >>= 1;
        m = (uint64_t)(r % (seq_t)o);
        g = gcd_u64(o, m);
        q = o / g;
        t = (2 * (uint64_t)k + 1) / q;
        r = r / (seq_t)g;
        r = r * (seq_t)t;
    }
    return r;
}
