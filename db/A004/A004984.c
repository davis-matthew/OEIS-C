/*
 * A004984  a(n) = (2^n/n!)*Product_{k=0..n-1} (4*k - 1).
 * https://oeis.org/A004984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*(4*n-5)*a(n-1)/n, a(0) = 1 (i.e. a(n) =
 *            (2^n/n!)*Product_{k=0..n-1} (4*k-1))
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 23, i128 n <= 45,
 *            big any n
 * Helpers:   a004984_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004984_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t num = 2 * (4 * si - 5);
        seq_t g = a004984_gcd(result, si);
        seq_t d = si / g;
        /* gcd(result/g, d) = 1 and d divides (result/g)*num, so d divides num */
        result = (result / g) * (num / d);
    }
    return result;
}
