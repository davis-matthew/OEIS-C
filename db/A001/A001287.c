/*
 * A001287  a(n) = binomial coefficient C(n,10).
 * https://oeis.org/A001287
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,10) = Product_{i=1..10} (n-10+i)/i, computed
 *            multiplicatively
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 46, u64 n <= 386,
 *            u128 n <= 1000, big any n
 * Helpers:   a001287_gcd_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001287_gcd_u.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t i;
    if (n < 10) return 0;
    for (i = 1; i <= 10; i++) {
        idx_t m = n - 10 + i;
        unsigned rm = (unsigned)(r % (seq_t)i);
        unsigned g = a001287_gcd_u((unsigned)i, rm);
        idx_t t;
        r /= (seq_t)g;
        t = i / g;
        /* gcd(r, t) == 1 and t divides r*m, so t divides m */
        r *= (seq_t)(m / t);
    }
    return r;
}
