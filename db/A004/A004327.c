/*
 * A004327  Binomial coefficient C(3n,n-9).
 * https://oeis.org/A004327
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n, n-9) = Product_{i=1..n-9} (2n+9+i)/i
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 17, u64 n <= 28, u128 n <= 51,
 *            big any n
 * Helpers:   a004327_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004327_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 9;
    idx_t base = 2 * n + 9;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(base + i);
        seq_t den = (seq_t)i;
        seq_t g = a004327_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
