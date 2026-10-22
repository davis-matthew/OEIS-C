/*
 * A005885  Theta series of f.c.c. lattice with respect to triangle.
 * https://oeis.org/A005885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of (x,y,z) in Z^3 with x+y+z even and (3x-2)^2 +
 *            (3y-1)^2 + (3z-1)^2 = 6(n+1); equivalently, the number of
 *            (u,v,w) with u,v,w == 1 (mod 3) and u^2 + v^2 + w^2 = 6(n+1).
 *            Computed by searching over u and v and testing w with an integer
 *            sqrt
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   a005885_is1mod3, a005885_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005885_is1mod3.h"
#include "../utils/a005885_isqrt.h"

seq_t a(idx_t n) {
    long long N = 6 * ((long long)n + 1);
    long long M = a005885_isqrt(N);
    long long count = 0;
    long long u, v, r, s;
    for (u = -M; u <= M; u++) {
        long long ru;
        if (!a005885_is1mod3(u)) continue;
        ru = N - u * u;
        if (ru < 0) continue;
        for (v = -M; v <= M; v++) {
            if (!a005885_is1mod3(v)) continue;
            r = ru - v * v;
            if (r < 0) continue;
            s = a005885_isqrt(r);
            if (s * s != r) continue;
            if (s == 0) {
                if (a005885_is1mod3(0)) count++;
            } else {
                if (a005885_is1mod3(s)) count++;
                if (a005885_is1mod3(-s)) count++;
            }
        }
    }
    {
        seq_t res = 0;
        long long i;
        for (i = 0; i < count; i++) res++;
        return res;
    }
}
