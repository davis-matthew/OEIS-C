/*
 * A004373  Binomial coefficient C(7n,n-5).
 * https://oeis.org/A004373
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(7*n, n-5)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 19, u128 n <= 23,
 *            big any n
 * Helpers:   a004373_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004373_gcd.h"

seq_t a(idx_t n) {
    idx_t k, i;
    seq_t r = 1;
    if (n < 5) return 0;
    k = n - 5;
    /* C(7n, k) built as C(6n+5+i, i) for i = 1..k; each step is exact */
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(6 * n + 5) + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a004373_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
