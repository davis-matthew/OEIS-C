/*
 * A004987  a(n) = (3^n/n!)*Product_{k=0..n-1} (3*k + 1). 3-central binomial
 *          coefficients.
 * https://oeis.org/A004987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*3*(3*n-2)/n, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 196 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t g = gcd(result, si);
        seq_t den = si / g;
        seq_t num = (seq_t)3 * ((seq_t)3 * si - 2);
        result = (result / g) * (num / den);
    }
    return result;
}
