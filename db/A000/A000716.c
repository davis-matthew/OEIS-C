/*
 * A000716  Number of partitions of n into parts of 3 kinds.
 * https://oeis.org/A000716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = Sum_{k>=1} (-1)^(k+1)*(2k+1)*a(n - k(k+1)/2),
 *            from Jacobi's identity Product_{m>=1} (1-x^m)^3 = Sum_{k>=0}
 *            (-1)^k*(2k+1)*x^(k(k+1)/2)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 47, u64 n <= 149, u128 n <= 512,
 *            big any n
 * Helpers:   a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_grow.h"

seq_t a(idx_t n) {
    static seq_t *c = NULL;
    static idx_t len = 0;

    if (n >= len) {
        idx_t newlen = len * 2;
        if (newlen < n + 1) newlen = n + 1;
        if (newlen < 64) newlen = 64;
        c = a306_grow(c, len, newlen);
        for (idx_t i = len; i < newlen; i++) {
            if (i == 0) {
                c[0] = 1;
                continue;
            }
            seq_t pos = 0, neg = 0;
            for (idx_t k = 1; k * (k + 1) / 2 <= i; k++) {
                idx_t t = k * (k + 1) / 2;
                seq_t term = (seq_t)(2 * k + 1) * c[i - t];
                if (k & 1) pos += term;
                else neg += term;
            }
            c[i] = pos - neg;
        }
        len = newlen;
    }
    return c[n];
}
