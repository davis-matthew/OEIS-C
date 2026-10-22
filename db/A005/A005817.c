/*
 * A005817  a(n) = C(floor(n/2 + 1/2))*C(floor(n/2 + 1)) where C(i) = Catalan
 *          numbers A000108.
 * https://oeis.org/A005817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(floor((n+1)/2)) * C(floor(n/2)+1), C(k) = Catalan
 *            numbers A000108 computed by C(0) = 1, C(k+1) = C(k)*(4k+2)/(k+2)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 38, u128 n <= 71,
 *            big any n
 * Helpers:   a005817_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005817_gcd.h"

seq_t a(idx_t n) {
    idx_t i1 = (n + 1) / 2;  /* floor(n/2 + 1/2) */
    idx_t i2 = n / 2 + 1;    /* floor(n/2 + 1), i2 >= i1 */
    seq_t c = 1, c1 = 1;
    idx_t k;
    if (i1 == 0) c1 = c;
    for (k = 0; k < i2; k++) {
        uint64_t num = 4 * (uint64_t)k + 2;
        uint64_t den = (uint64_t)k + 2;
        uint64_t g = a005817_gcd(num, den);
        c /= (seq_t)(den / g);
        c *= (seq_t)(num / g);
        if (k + 1 == i1) c1 = c;
    }
    return c1 * c;
}
