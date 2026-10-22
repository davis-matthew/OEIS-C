/*
 * A000224  Number of squares mod n.
 * https://oeis.org/A000224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   multiplicative with a(p^e) = floor(p^e/6) + 2 if p = 2;
 *            floor(p^(e+1)/(2p + 2)) + 1 if p > 2
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a224_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a224_local.h"

seq_t a(idx_t n)
{
    seq_t r = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    if (m <= 1)
        return r;
    for (p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t q = 1;
            while (m % p == 0) {
                m /= p;
                q *= p;
            }
            r *= (seq_t)a224_local(p, q);
        }
    }
    if (m > 1)
        r *= (seq_t)a224_local(m, m);
    return r;
}
