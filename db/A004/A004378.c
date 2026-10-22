/*
 * A004378  Binomial coefficient C(7n,n-10).
 * https://oeis.org/A004378
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(7n, n-10) = Product_{i=1..n-10} (6n+10+i)/i
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 16, u64 n <= 23, u128 n <= 27,
 *            big any n
 * Helpers:   a004378_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004378_gcd.h"

seq_t a(idx_t n) {
    idx_t m = 7 * n;
    idx_t k = n - 10;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(m - k + i);
        seq_t d = (seq_t)i;
        seq_t g = a004378_gcd(r, d);
        r /= g;
        d /= g;
        r *= num / d;
    }
    return r;
}
