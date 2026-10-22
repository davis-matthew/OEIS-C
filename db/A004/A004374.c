/*
 * A004374  Binomial coefficient C(7n,n-6).
 * https://oeis.org/A004374
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(7*n, n-6), computed multiplicatively as
 *            prod_{i=1..n-6} (6n+6+i)/i
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 12, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   a004374_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004374_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 6;
    idx_t base = 6 * n + 6;   /* 7n - k */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(base + i);
        seq_t den = (seq_t)i;
        seq_t g = a004374_gcd(r, den);
        seq_t r1 = r / g;
        seq_t d1 = den / g;
        /* d1 divides num since r*num/den is an integer and gcd(r1,d1)=1 */
        r = r1 * (num / d1);
    }
    return r;
}
