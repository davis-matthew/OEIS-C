/*
 * A000332  Binomial coefficient binomial(n,4) = n*(n-1)*(n-2)*(n-3)/24.
 * https://oeis.org/A000332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(n,4) = n*(n-1)*(n-2)*(n-3)/24
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 568, u64 n <= 1002,
 *            u128 n <= 1002, big any n
 * Helpers:   a000332_small_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000332_small_gcd.h"

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t k;
    if (n < 4)
        return (seq_t)0;
    /* r = C(n-4+k, k) after step k; divide before multiplying to avoid overflow */
    for (k = 1; k <= 4; k++) {
        idx_t m = n - 4 + k;
        idx_t g = a000332_small_gcd(m % k, k);
        r = r / (seq_t)(k / g);
        r = r * (seq_t)(m / g);
    }
    return r;
}
