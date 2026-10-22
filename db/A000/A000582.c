/*
 * A000582  a(n) = binomial coefficient C(n,9).
 * https://oeis.org/A000582
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n,9), computed as C(n,i+1) = C(n,i)*(n-i)/(i+1) with
 *            gcd reduction to avoid overflow
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 52, u64 n <= 577,
 *            u128 n <= 1000, big any n
 * Helpers:   a582_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a582_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i < 9; i++) {
        unsigned long long d = (unsigned long long)(i + 1);
        unsigned long long r = (unsigned long long)(result % (seq_t)d);
        unsigned long long g = a582_gcd(r, d);
        idx_t m = (idx_t)((n - i) / (idx_t)(d / g));
        result = (result / (seq_t)g) * (seq_t)m;
    }
    return result;
}
