/*
 * A004362  Binomial coefficient C(6n,n-7).
 * https://oeis.org/A004362
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(6*n, n-7) = Product_{i=1..n-7} (5*n+7+i)/i
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 13, u64 n <= 21, u128 n <= 25,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
    idx_t k = n - 7;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(5 * n + 7 + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq(r, den);
        r /= g;
        den /= g;
        /* den now divides num, since r*num/i is an integer and gcd(r, den) = 1 */
        r *= num / den;
    }
    return r;
}
