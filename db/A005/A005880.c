/*
 * A005880  Theta series of D_4 lattice with respect to edge.
 * https://oeis.org/A005880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer (x,y,z,w) with x+y+z+w even and
 *            (2x+1)^2 + (2y+1)^2 + 4z^2 + 4w^2 = 4n+2 (D_4 points at squared
 *            distance n+1/2 from the edge midpoint (1/2,1/2,0,0)); counted by
 *            direct enumeration over x, y, z, solving for w
 * Range:     u8 n <= 45, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   a005880_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005880_isqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    long long M = 4LL * (long long)n + 2;
    long long L = a005880_isqrt(M) + 1;
    long long x, y, z;

    for (x = -L; x <= L; x++) {
        long long u = 2 * x + 1;
        long long A = u * u;
        if (A > M) continue;
        for (y = -L; y <= L; y++) {
            long long v = 2 * y + 1;
            long long B = A + v * v;
            if (B > M) continue;
            for (z = -L; z <= L; z++) {
                long long C = B + 4 * z * z;
                long long r, q, w;
                if (C > M) continue;
                r = M - C;
                if (r % 4 != 0) continue;
                q = r / 4;
                w = a005880_isqrt(q);
                if (w * w != q) continue;
                if (((x + y + z + w) & 1) != 0) continue;
                if (w == 0) count += 1;
                else count += 2;
            }
        }
    }
    return count;
}
