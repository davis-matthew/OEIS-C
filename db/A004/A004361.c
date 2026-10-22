/*
 * A004361  Binomial coefficient C(6n,n-6).
 * https://oeis.org/A004361
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(6n, n-6) = Product_{i=1..n-6} (5n+6+i)/i
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 37,
 *            big any n
 * Helpers:   a004361_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004361_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 6;
    idx_t base = 5 * n + 6; /* 6n - k */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(base + i);
        seq_t den = (seq_t)i;
        seq_t g = a004361_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
