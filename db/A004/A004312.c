/*
 * A004312  Binomial coefficient C(2n,n-6).
 * https://oeis.org/A004312
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-6) = Product_{i=1..n-6} (n+6+i)/i
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   a004312_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004312_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 6;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        idx_t m = n + 6 + i;
        /* r*m is divisible by i; split to avoid overflow beyond the result */
        idx_t rm = (idx_t)(r % (seq_t)i);
        idx_t g = a004312_gcd(i, rm);
        idx_t d = i / g;
        r /= (seq_t)g;
        r *= (seq_t)(m / d);
    }
    return r;
}
