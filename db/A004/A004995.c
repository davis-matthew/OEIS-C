/*
 * A004995  a(n) = (6^n/n!) * Product_{k=0..n-1} (6*k - 5).
 * https://oeis.org/A004995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*(6*n-11)*a(n-1)/n, a(0) = 1 (i.e. a(n) = (6^n/n!) *
 *            Product_{k=0..n-1} (6*k - 5))
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 26,
 *            big any n
 * Helpers:   a004995_gcd_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004995_gcd_ll.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        long long num = 6LL * (6LL * (long long)i - 11LL);
        long long den = (long long)i;
        long long g = a004995_gcd_ll(num, den);
        num /= g;
        den /= g;
        /* result * num is divisible by den, and gcd(num, den) = 1, so den | result */
        result = result / (seq_t)den;
        result = result * (seq_t)num;
    }
    return result;
}
