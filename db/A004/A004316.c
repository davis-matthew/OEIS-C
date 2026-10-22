/*
 * A004316  a(n) = binomial coefficient C(2n, n-10).
 * https://oeis.org/A004316
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-10) = C(2n, n+10) = Product_{i=1..n-10}
 *            (n+10+i)/i
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 21, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   a004316_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004316_gcd.h"

seq_t a(idx_t n) {
    idx_t k, i;
    seq_t r = 1;
    if (n < 10) return 0;
    k = n - 10;
    for (i = 1; i <= k; i++) {
        idx_t m = n + 10 + i;
        idx_t rm = (idx_t)(r % (seq_t)i);
        idx_t g = a004316_gcd(i, rm);
        idx_t d = i / g;
        r /= (seq_t)g;
        r *= (seq_t)(m / d);
    }
    return r;
}
