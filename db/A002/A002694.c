/*
 * A002694  Binomial coefficients C(2n, n-2).
 * https://oeis.org/A002694
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-2) = Product_{i=1..n-2} (n+2+i)/i, computed
 *            multiplicatively with gcd reduction
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   a002694_sgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002694_sgcd.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;
    idx_t k = n - 2;
    idx_t base = n + 2; /* m - k where m = 2n */
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(base + i);
        seq_t den = (seq_t)i;
        seq_t g = a002694_sgcd(result, den);
        result /= g;
        den /= g;
        result *= num / den;
    }
    return result;
}
