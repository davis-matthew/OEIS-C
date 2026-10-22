/*
 * A003517  Number of permutations of [n+1] with exactly 1 increasing
 *          subsequence of length 3.
 * https://oeis.org/A003517
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) * 2*n*(2*n+1) / ((n+4)*(n-2)), a(2) = 1 (i.e. a(n)
 *            = 6*binomial(2*n+1, n-2)/(n+4))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   a3517_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3517_gcd.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;

    seq_t result = 1;
    for (idx_t k = 3; k <= n; k++) {
        uint64_t num = 2 * (uint64_t)k * (2 * (uint64_t)k + 1);
        uint64_t den = ((uint64_t)k + 4) * ((uint64_t)k - 2);
        uint64_t g = a3517_gcd(num, den);
        num /= g;
        den /= g;
        /* den now divides result exactly, since result*num/den is an integer and gcd(num,den)=1 */
        result = result / (seq_t)den * (seq_t)num;
    }
    return result;
}
