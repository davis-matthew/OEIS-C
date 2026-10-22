/*
 * A003177  a(n) = ceiling(24(2^n-1)/n).
 * https://oeis.org/A003177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(24*(2^n-1)/n); computed as 24*q + ceiling(24*r/n)
 *            where 2^n-1 = q*n + r
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t q = 0;
    idx_t r = 0;
    idx_t i;
    /* long division of (2^n - 1) (n one-bits) by n */
    for (i = 0; i < n; i++) {
        r = 2 * r + 1;
        q = q * 2;
        if (r >= n) {
            r -= n;
            q += 1;
        }
    }
    {
        uint64_t t = (uint64_t)24 * r;
        uint64_t c = (t + n - 1) / n;
        return q * 24 + (seq_t)c;
    }
}
