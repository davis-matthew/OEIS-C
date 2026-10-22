/*
 * A004334  Binomial coefficient C(4n,n-4).
 * https://oeis.org/A004334
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n, n-4), computed multiplicatively:
 *            prod_{i=0}^{n-5} (4n-i)/(i+1), with gcd reduction so that no
 *            intermediate value exceeds the final result
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   a004334_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004334_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i + 4 < n; i++) {
        seq_t num = 4 * (seq_t)n - (seq_t)i;
        seq_t den = (seq_t)i + 1;
        seq_t g = a004334_gcd(result % den, den);
        seq_t r = result / g;
        seq_t d = den / g;
        result = r * (num / d);
    }
    return result;
}
