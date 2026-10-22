/*
 * A004996  a(n) = 6^n/n! * Product_{k=0..n-1} (6*k - 1).
 * https://oeis.org/A004996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*(6*n-7)*a(n-1)/n, a(0) = 1 (i.e. a(n) = 6^n/n! *
 *            Product_{k=0..n-1} (6*k - 1))
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 26,
 *            big any n
 * Helpers:   a004996_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004996_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t i_s = (seq_t)i;
        seq_t m = (i_s * 6 - 7) * 6;
        /* result * m / i is exact; cancel the common factors before multiplying */
        seq_t g = a004996_gcd(result, i_s);
        seq_t r = result / g;
        seq_t d = i_s / g;   /* d is coprime to r, so d divides m */
        result = r * (m / d);
    }
    return result;
}
