/*
 * A004994  a(n) = (6^n/n!)*Product_{k=0..n-1} (6*k + 5).
 * https://oeis.org/A004994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*6*(6*n-1)/n, a(0) = 1 (i.e. a(n) =
 *            Product_{k=1..n} (36 - 6/k))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   a004994_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004994_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t g = a004994_gcd(result, si);
        if (g == 0) g = 1;
        seq_t rest = si / g;               /* rest divides 6*(6i-1) */
        if (rest == 0) rest = 1;
        seq_t m = (seq_t)6 * ((seq_t)6 * si - 1) / rest;
        result = (result / g) * m;
    }
    return result;
}
