/*
 * A006208  Generalized Fibonacci numbers A_{n,3}.
 * https://oeis.org/A006208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} mu(n/d) * (s(d) - (-1)^d), where s(n)
 *            is the power sum of the roots of x^7 - 2x^5 - 1: s(n) = 2*s(n-2)
 *            + s(n-7), s(1..7) = 0,4,0,8,0,16,7 (primitive period-n orbit
 *            count for the period-7 Stefan cycle map)
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static int mobius(uint64_t m) {
    int r = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

seq_t a(idx_t n) {
    idx_t len = (n < 8 ? 8 : n) + 1;
    seq_t *s = alloc_zero(len);
    idx_t i, d;
    seq_t pos = 0, neg = 0, res;
    s[1] = 0; s[2] = 4; s[3] = 0; s[4] = 8; s[5] = 0; s[6] = 16; s[7] = 7;
    for (i = 8; i <= n; i++)
        s[i] = 2 * s[i - 2] + s[i - 7];
    for (d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        int mu = mobius((uint64_t)(n / d));
        if (mu == 0) continue;
        if (mu > 0) {
            pos += s[d];
            if (d & 1) pos += 1; else neg += 1;
        } else {
            neg += s[d];
            if (d & 1) neg += 1; else pos += 1;
        }
    }
    res = (pos - neg) / (seq_t)n;
    release(s, len);
    return res;
}
