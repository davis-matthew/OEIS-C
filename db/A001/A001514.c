/*
 * A001514  Bessel polynomial {y_n}'(1).
 * https://oeis.org/A001514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n+k)!/((n-k)!*(k-1)!*2^k), a(0) = 0;
 *            computed via t(1) = n(n+1)/2, t(k+1) = t(k)*(n+k+1)*(n-k)/(2k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   a1514_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1514_gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t sum = t;
    for (idx_t k = 1; k < n; k++) {
        uint64_t num = (uint64_t)(n + k + 1) * (uint64_t)(n - k);
        uint64_t den = 2 * (uint64_t)k;
        uint64_t g = a1514_gcd(num, den);
        num /= g;
        den /= g;
        t /= (seq_t)den;
        t *= (seq_t)num;
        sum += t;
    }
    return sum;
}
