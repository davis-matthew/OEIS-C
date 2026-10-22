/*
 * A002734  Remove squares!
 * https://oeis.org/A002734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007913(A000037(n)), where A000037(n) = n + floor(1/2 +
 *            sqrt(n)) and A007913 is the squarefree part
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2734_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2734_isqrt.h"

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t s = a2734_isqrt(nn);
    uint64_t r = (nn - s * s > s) ? s + 1 : s;
    uint64_t m = nn + r;

    uint64_t core = 1;
    uint64_t t = m;
    uint64_t p;
    for (p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                e ^= 1;
            }
            if (e) core *= p;
        }
    }
    if (t > 1) core *= t;

    return (seq_t)core;
}
