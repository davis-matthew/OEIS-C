/*
 * A000581  a(n) = binomial coefficient C(n,8).
 * https://oeis.org/A000581
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,8) =
 *            n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6)*(n-7)/8!
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 63, u64 n <= 967,
 *            u128 n <= 1000, big any n
 * Helpers:   a000581_gcd_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000581_gcd_small.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t i;
    for (i = 0; i < 8; i++) {
        /* r = C(n, i); make it r * (n - i) / (i + 1) without overflow */
        unsigned k = (unsigned)(i + 1);
        unsigned rm = (unsigned)(r % k);
        unsigned g = a000581_gcd_small(rm == 0 ? k : rm, k);
        unsigned d = k / g;
        r /= g;
        /* gcd(r/g, d) = 1 and d divides r*(n-i), so d divides n-i */
        r *= (seq_t)((n - i) / d);
    }
    return r;
}
