/*
 * A004371  Binomial coefficient C(7n,n-3).
 * https://oeis.org/A004371
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(7n, n-3), computed multiplicatively as C(6n+3+i, i) for
 *            i = 1..n-3, reducing by gcd so no intermediate exceeds the
 *            result
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   a004371_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004371_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 3;
    idx_t base = 6 * n + 3;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)base + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a004371_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
