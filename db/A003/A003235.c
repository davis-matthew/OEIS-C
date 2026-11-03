/*
 * A003235  a(n) = Sum_{k=0..n} (-1)^(n-k) C(n,k)*C(k^2,n).
 * https://oeis.org/A003235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * C(n,k) * C(k^2,n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 277 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static unsigned __int128 a3235_inv(unsigned __int128 x) {
    unsigned __int128 y = x;
    int i;
    for (i = 0; i < 7; i++) y *= (unsigned __int128)2 - x * y;
    return y;
}

/* C(m, r) modulo 2^128 */
static unsigned __int128 a3235_bmod(uint64_t m, uint64_t r) {
    unsigned __int128 num = 1, den = 1;
    long e = 0;
    uint64_t i;
    if (r > m) return 0;
    for (i = 0; i < r; i++) {
        uint64_t x = m - i, y = i + 1;
        while (!(x & 1)) { x >>= 1; e++; }
        while (!(y & 1)) { y >>= 1; e--; }
        num *= x;
        den *= y;
    }
    if (e >= 128) return 0;
    return (num * a3235_inv(den)) << e;
}

static int a3235_fixed(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x *= 2;
    return x == 0;
}

seq_t a(idx_t n) {
    idx_t k;
    if (a3235_fixed()) {
        unsigned __int128 total = 0;
        seq_t r = 0;
        int b;
        for (k = 0; k <= n; k++) {
            unsigned __int128 t;
            if (k * k < n) continue;
            t = a3235_bmod(n, k) * a3235_bmod(k * k, n);
            if (((n - k) & 1) == 0) total += t;
            else total -= t;
        }
        for (b = 127; b >= 0; b--) {
            r = r * 2;
            if ((total >> b) & 1) r += 1;
        }
        return r;
    } else {
        seq_t pos = 0, neg = 0, cnk = 1;
        for (k = 0; k <= n; k++) {
            if (k > 0) cnk = cnk * (seq_t)(n - k + 1) / (seq_t)k;
            if (k * k >= n) {
                idx_t m = k * k, j;
                seq_t c = 1, t;
                for (j = 0; j < n; j++) c = c * (seq_t)(m - j) / (seq_t)(j + 1);
                t = cnk * c;
                if (((n - k) & 1) == 0) pos += t;
                else neg += t;
            }
        }
        return pos - neg;
    }
}
