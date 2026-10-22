/*
 * A004989  a(n) = (3^n/n!) * Product_{k=0..n-1} (3*k - 2).
 * https://oeis.org/A004989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*(3*n-5)*a(n-1)/n, a(0) = 1
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 22, i128 n <= 43,
 *            big any n
 * Helpers:   a004989_gcd_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004989_gcd_ll.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        long long d = (long long)i;
        long long m = 3 * (3 * d - 5);
        long long g = a004989_gcd_ll(m, d);
        m /= g;
        d /= g;
        /* d divides result exactly since gcd(m, d) = 1 */
        result = (result / (seq_t)d) * (seq_t)m;
    }
    return result;
}
