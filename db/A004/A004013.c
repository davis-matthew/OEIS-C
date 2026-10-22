/*
 * A004013  Theta series of body-centered cubic (b.c.c.) lattice.
 * https://oeis.org/A004013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of integer triples (x,y,z), all even or all odd,
 *            with x^2+y^2+z^2 = n (theta_3(q)^3 + theta_2(q)^3 in powers of
 *            q^(1/4))
 * Range:     u8 n <= 658, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004013_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004013_isqrt.h"

seq_t a(idx_t n) {
    seq_t cnt = 0;
    long long N = (long long)n;
    long long s = (long long)a004013_isqrt((unsigned long long)n);
    long long x, y;
    for (x = -s; x <= s; x++) {
        long long rx = N - x * x;
        if (rx < 0) continue;
        for (y = -s; y <= s; y++) {
            long long r = rx - y * y;
            long long z;
            if (r < 0) continue;
            if (((x - y) & 1) != 0) continue;
            z = (long long)a004013_isqrt((unsigned long long)r);
            if (z * z != r) continue;
            if (((z - x) & 1) != 0) continue;
            if (z == 0) cnt += 1;
            else cnt += 2;
        }
    }
    return cnt;
}
