/*
 * A004998  a(n) = (6^n/n!) * Product_{k=0..n-1} ( 6*k + 11 ).
 * https://oeis.org/A004998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(6*n+5)*a(n-1)/n, a(0) = 1 (i.e. a(n) = (6^n/n!) *
 *            Product_{k=0..n-1} (6*k+11))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   a004998_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004998_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t iv = (seq_t)i;
        seq_t g = a004998_gcd(result, iv);
        seq_t den = iv / g;
        seq_t num = (seq_t)6 * ((seq_t)6 * iv + 5) / den;
        result = (result / g) * num;
    }
    return result;
}
