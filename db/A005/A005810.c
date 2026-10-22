/*
 * A005810  a(n) = binomial(4n,n).
 * https://oeis.org/A005810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = binomial(4n,n) = Product_{i=1..n} (3n+i)/i
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   a005810_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005810_gcd_u64.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        /* r currently equals C(3n+i-1, i-1); next is r*(3n+i)/i */
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = a005810_gcd_u64(rm, (uint64_t)i);
        if (g == 0) g = (uint64_t)i;
        r /= (seq_t)g;
        {
            uint64_t d = (uint64_t)i / g;
            seq_t num = (seq_t)(3 * n + i);
            num /= (seq_t)d;
            r *= num;
        }
    }
    return r;
}
