/*
 * A004992  a(n) = (3^n/n!) * Product_{k=0..n-1} (3*k + 5).
 * https://oeis.org/A004992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3*n+2)*a(n-1)/n, a(0) = 1 (i.e. a(n) = (3^n/n!) *
 *            Product_{k=0..n-1} (3*k+5))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   a004992_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004992_gcd.h"

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t g = a004992_gcd(si, v);
        if (g == 0) g = 1;
        seq_t d = si / g;                 /* d divides 3*(3i+2) */
        if (d == 0) d = 1;
        seq_t m = ((seq_t)3 * ((seq_t)3 * si + 2)) / d;
        v = (v / g) * m;
    }
    return v;
}
