/*
 * A004353  Binomial coefficient C(5n,n-11).
 * https://oeis.org/A004353
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(5n, n-11), computed multiplicatively as
 *            prod_{i=1..n-11} (4n+11+i)/i with gcd reduction so no
 *            intermediate exceeds the result
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 17, u64 n <= 26, u128 n <= 29,
 *            big any n
 * Helpers:   a4353_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a4353_gcd.h"

seq_t a(idx_t n) {
    if (n < 11) return 0;
    idx_t k = n - 11;
    seq_t base = (seq_t)4 * (seq_t)n + 11;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; i++) {
        seq_t num = base + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a4353_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
