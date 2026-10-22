/*
 * A002737  a(n) = Sum_{j=0..n} (n+j)*binomial(n+j,j).
 * https://oeis.org/A002737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*binomial(2*n+4, n+2)/4 = n*binomial(2*n+3, n+1)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   a002737_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002737_gcd.h"

seq_t a(idx_t n) {
    idx_t m = 2 * n + 3;
    idx_t k;
    seq_t c = 1;
    /* c = binomial(m, k), built multiplicatively; intermediate values never
       exceed the final binomial */
    for (k = 1; k <= n + 1; k++) {
        idx_t num = m - k + 1;
        idx_t g = a002737_gcd(num, k);
        idx_t t = num / g;
        idx_t kk = k / g;
        c = (c / kk) * t;
    }
    if (n % 2 == 0)
        return (seq_t)(n / 2) * c;
    return (c / 2) * (seq_t)n;
}
