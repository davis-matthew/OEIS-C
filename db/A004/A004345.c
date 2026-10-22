/*
 * A004345  Binomial coefficient C(5n,n-3).
 * https://oeis.org/A004345
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n, n-3) = Product_{i=0..n-4} (5*n-i)/(i+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   a004345_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004345_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 3;
    seq_t result = 1;

    for (idx_t i = 0; i < k; i++) {
        uint64_t num = (uint64_t)(5 * n - i);
        uint64_t den = (uint64_t)(i + 1);
        uint64_t g = a004345_gcd(num, den);
        num /= g;
        den /= g;
        /* result * num / den is an integer and gcd(num, den) = 1,
           so den divides result exactly */
        result /= (seq_t)den;
        result *= (seq_t)num;
    }

    return result;
}
