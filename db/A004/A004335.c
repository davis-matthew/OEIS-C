/*
 * A004335  Binomial coefficient C(4n,n-5).
 * https://oeis.org/A004335
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(4n, n-5) = Product_{i=1..n-5} (3n+5+i)/i
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   a004335_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004335_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 5;
    seq_t base = (seq_t)3 * (seq_t)n + 5; /* 4n - k */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = C(base+i-1, i-1); update to C(base+i, i) = r * (base+i) / i */
        seq_t num = base + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a004335_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
