/*
 * A004015  Theta series of face-centered cubic (f.c.c.) lattice.
 * https://oeis.org/A004015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A005875(2*n), the number of ways to write 2*n as x^2 +
 *            y^2 + z^2 with x, y, z in Z (search over x, y; z determined)
 * Range:     u8 n <= 156, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   a004015_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004015_isqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    long long m = 2 * (long long)n;
    long long r = (long long)a004015_isqrt((uint64_t)m);
    long long x, y;
    for (x = -r; x <= r; x++) {
        long long rx = m - x * x;
        if (rx < 0) continue;
        for (y = -r; y <= r; y++) {
            long long rem = rx - y * y;
            long long s;
            if (rem < 0) continue;
            s = (long long)a004015_isqrt((uint64_t)rem);
            if (s * s != rem) continue;
            if (s == 0) count += 1;
            else count += 2;
        }
    }
    return count;
}
