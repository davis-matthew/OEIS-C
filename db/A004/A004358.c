/*
 * A004358  Binomial coefficient C(6n,n-3).
 * https://oeis.org/A004358
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(6*n, n-3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   gcd_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_ull.h"

seq_t a(idx_t n) {
    idx_t k = n - 3;
    idx_t base = 6 * n - k;   /* = 5n + 3 */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = C(base+i-1, i-1); next = r * (base+i) / i, kept exact without overflow */
        unsigned long long num = (unsigned long long)(base + i);
        unsigned long long rm = (unsigned long long)(r % (seq_t)i);
        unsigned long long g = gcd_ull(rm, (unsigned long long)i);
        unsigned long long d = (unsigned long long)i / g;
        r = (r / (seq_t)g) * (seq_t)(num / d);
    }
    return r;
}
