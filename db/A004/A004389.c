/*
 * A004389  a(n) = binomial(8*n, n-8).
 * https://oeis.org/A004389
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(8*n, n-8)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   a004389_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004389_gcd.h"

seq_t a(idx_t n) {
    idx_t m = 8 * n;
    idx_t k = n - 8;
    seq_t r = 1;
    idx_t i;
    /* r = binomial(m-k+i, i) after step i; reduce by gcd so the
       intermediate product never exceeds the next binomial value */
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(m - k + i);
        seq_t d = (seq_t)i;
        seq_t g = a004389_gcd(r, d);
        r /= g;
        d /= g;
        num /= d;
        r *= num;
    }
    return r;
}
