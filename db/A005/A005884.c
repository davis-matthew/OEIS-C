/*
 * A005884  Theta series of f.c.c. lattice with respect to edge.
 * https://oeis.org/A005884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A045828(n) = number of f.c.c. lattice points (x,y,z),
 *            x+y+z even, with (2x-1)^2 + (2y-1)^2 + 4z^2 = 4n+2 (squared
 *            distances from the edge midpoint (1/2,1/2,0)); counted by
 *            enumerating x, y and solving for z
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 91 terms through n = 973 (2026-11-01)
 * Helpers:   isqrt_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_i64.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    long long N = 4 * (long long)n + 2;
    long long U = isqrt_i64(N) + 2;
    long long u, v;
    for (u = -U; u <= U; u++) {
        long long uu, x;
        if ((u & 1) == 0) continue;
        uu = u * u;
        if (uu > N) continue;
        x = (u + 1) / 2;
        for (v = -U; v <= U; v++) {
            long long vv, rem, r, z, y;
            if ((v & 1) == 0) continue;
            vv = v * v;
            if (uu + vv > N) continue;
            rem = N - uu - vv;
            if (rem % 4 != 0) continue;
            r = rem / 4;
            z = isqrt_i64(r);
            if (z * z != r) continue;
            y = (v + 1) / 2;
            if ((x + y + z) % 2 == 0) {
                if (z == 0) count += 1;
                else count += 2;
            }
        }
    }
    return count;
}
