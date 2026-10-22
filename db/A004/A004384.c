/*
 * A004384  Binomial coefficient C(8n,n-3).
 * https://oeis.org/A004384
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(8n, n-3) = Product_{i=1..n-3} (7n+3+i)/i
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   gcd_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_val.h"

seq_t a(idx_t n) {
    idx_t k = n - 3;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(7 * n + 3 + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_val(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
