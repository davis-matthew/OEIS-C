/*
 * A004387  Binomial coefficient C(8n,n-6).
 * https://oeis.org/A004387
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(8*n, n-6)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 12, u64 n <= 19, u128 n <= 23,
 *            big any n
 * Helpers:   a004387_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004387_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 6;
    idx_t base = 7 * n + 6; /* 8n - k */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = C(base + i - 1, i - 1) -> C(base + i, i) without overflow */
        seq_t num = (seq_t)base + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = a004387_gcd(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
