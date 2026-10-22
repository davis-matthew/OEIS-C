/*
 * A004332  a(n) = C(4n,n-2).
 * https://oeis.org/A004332
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(4n,n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   a004332_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004332_gcd.h"

seq_t a(idx_t n) {
    idx_t k = n - 2;
    idx_t base = 3 * n + 2; /* 4n - k */
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(base + i);
        seq_t den = (seq_t)i;
        seq_t g = a004332_gcd(r, den);
        r = r / g;
        den = den / g;
        r = r * (num / den);
    }
    return r;
}
