/*
 * A003409  a(n) = 3*binomial(2n-1,n).
 * https://oeis.org/A003409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*binomial(2n-1,n) = 3*Product_{i=1..n-1} (n+i)/i
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   a3409_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a3409_gcd.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    for (idx_t i = 1; i < n; i++) {
        seq_t num = (seq_t)n + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a3409_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r * 3;
}
