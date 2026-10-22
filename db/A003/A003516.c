/*
 * A003516  Binomial coefficients C(2n+1, n-2).
 * https://oeis.org/A003516
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n+1, n-2), computed multiplicatively: C(m,k) =
 *            prod_{i=1..k} (m-k+i)/i, m = 2n+1, k = n-2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   a3516_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3516_gcd.h"

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t k, i;
    if (n < 2) return 0;
    k = n - 2;
    for (i = 1; i <= k; i++) {
        /* r * (n+3+i) / i is exact; divide out the common factor first */
        uint64_t num = (uint64_t)(n + 3 + i);
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = a3516_gcd(rm, (uint64_t)i);
        uint64_t d;
        if (g == 0) g = (uint64_t)i;
        r /= (seq_t)g;
        d = (uint64_t)i / g;
        r *= (seq_t)(num / d);
    }
    return r;
}
