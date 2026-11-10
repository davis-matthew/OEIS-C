/*
 * A071017  Moebius transform of A002487.
 * https://oeis.org/A071017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|(n+1)} mu((n+1)/d) * A002487(d+1), with
 *            A002487(0)=0, A002487(1)=1, A002487(2n)=A002487(n),
 *            A002487(2n+1)=A002487(n)+A002487(n+1)
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Validated: partial, pass, 102 terms through n = 101 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A071017_stern(idx_t k) {
    seq_t x = 0, y = 1; /* (s(m), s(m+1)) for m = 0 */
    int bits = 0;
    idx_t t = k;
    while (t) { bits++; t >>= 1; }
    for (int i = bits - 1; i >= 0; i--) {
        if ((k >> i) & 1) { x = x + y; }
        else { y = x + y; }
    }
    return x;
}

static int A071017_mu(idx_t m) {
    int r = 1;
    for (idx_t p = 2; p * p <= m; p++) {
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
    idx_t N = n + 1;
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= N; d++) {
        if (N % d != 0) continue;
        idx_t e = N / d;
        int m1 = A071017_mu(e); /* divisor d, cofactor e */
        if (m1 != 0) {
            seq_t v = A071017_stern(d + 1);
            if (m1 > 0) sum += v; else sum -= v;
        }
        if (e != d) {
            int m2 = A071017_mu(d); /* divisor e, cofactor d */
            if (m2 != 0) {
                seq_t v = A071017_stern(e + 1);
                if (m2 > 0) sum += v; else sum -= v;
            }
        }
    }
    return sum;
}
