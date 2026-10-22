/*
 * A001288  a(n) = binomial(n,11).
 * https://oeis.org/A001288
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,11), computed as C(k+i,i) =
 *            C(k+i-1,i-1)*(k+i)/i for i=1..11 with k = n-11, cancelling gcds
 *            first so no intermediate exceeds the final value
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 42, u64 n <= 282,
 *            u128 n <= 1000, big any n
 * Helpers:   a001288_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001288_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 11;
    seq_t result = 1;
    for (idx_t i = 1; i <= 11; i++) {
        unsigned long long r = (unsigned long long)(result % (seq_t)i);
        unsigned long long g = a001288_gcd(r, (unsigned long long)i);
        idx_t d = (idx_t)(i / g);
        idx_t f = (k + i) / d;
        result = (result / (seq_t)g) * (seq_t)f;
    }
    return result;
}
