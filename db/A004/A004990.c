/*
 * A004990  a(n) = (3^n/n!)*Product_{k=0..n-1} (3*k - 1).
 * https://oeis.org/A004990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 22, i128 n <= 42,
 *            big any n
 * Helpers:   a004990_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004990_gcd.h"
/* Recurrence: n*a(n) = 3*(3*n-4)*a(n-1), a(0) = 1 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        long long num = 3LL * (3LL * (long long)i - 4LL);
        long long den = (long long)i;
        long long g = a004990_gcd(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num, so den divides result */
        result = result / (seq_t)den;
        result = result * (seq_t)num;
    }
    return result;
}
