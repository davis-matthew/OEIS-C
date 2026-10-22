/*
 * A002054  Binomial coefficient C(2n+1, n-1).
 * https://oeis.org/A002054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+1, n-1), computed multiplicatively as
 *            Product_{i=1..n-1} (n+2+i)/i with gcd reduction to keep
 *            intermediates exact
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   a002054_gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002054_gcd_local.h"

seq_t a(idx_t n) {
    idx_t m, k, i;
    seq_t r = 1;
    if (n < 1) return 0;
    m = 2 * n + 1;
    k = n - 1;
    for (i = 1; i <= k; i++) {
        idx_t num = m - k + i;
        idx_t den = i;
        idx_t g = a002054_gcd_local(num, den);
        num /= g;
        den /= g;
        /* r * num / den is an integer and gcd(num, den) = 1, so den | r */
        r = (r / den) * (seq_t)num;
    }
    return r;
}
