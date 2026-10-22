/*
 * A005754  Number of planted identity matched trees with n nodes.
 * https://oeis.org/A005754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1) = 1; a(n+1) = [x^n] Prod_{m>=1} (1+x^m)^b(m) (Weigh
 *            transform of b = A005753), where b(m) = Sum_{i+j=m+1} a(i)*a(j).
 *            Computed by c(m) = Sum_{d|m} (-1)^(m/d+1) * d * b(d), n*a(n+1) =
 *            Sum_{k=1..n} c(k)*a(n+1-k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 57,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

#ifdef __cplusplus
typedef seq_t W;
#else
typedef unsigned __int128 W;
#endif

static W *wgrow(W *old, idx_t oldlen, idx_t newlen) {
#ifdef __cplusplus
    return grow_zero(old, oldlen, newlen);
#else
    W *p = (W *)realloc(old, (size_t)newlen * sizeof(W));
    for (idx_t i = oldlen; i < newlen; i++) p[i] = 0;
    return p;
#endif
}

static W *Aa = NULL, *Bb = NULL, *Cc = NULL;
static idx_t cap = 0, have = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n + 2 > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < n + 2) nc *= 2;
        Aa = wgrow(Aa, cap, nc);
        Bb = wgrow(Bb, cap, nc);
        Cc = wgrow(Cc, cap, nc);
        cap = nc;
    }
    if (have == 0) {
        Aa[1] = 1;
        have = 1;
    }
    while (have < n) {
        idx_t m = have;
        /* b(m) */
        W s = 0;
        for (idx_t i = 1; 2 * i < m + 1; i++) s += Aa[i] * Aa[m + 1 - i];
        s += s;
        if ((m + 1) % 2 == 0) s += Aa[(m + 1) / 2] * Aa[(m + 1) / 2];
        Bb[m] = s;
        /* c(m) */
        W pos = 0, neg = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d) continue;
            idx_t e = m / d;
            if (e & 1) pos += Bb[d] * (W)d; else neg += Bb[d] * (W)d;
            if (e != d) {
                if (d & 1) pos += Bb[e] * (W)e; else neg += Bb[e] * (W)e;
            }
        }
        Cc[m] = pos - neg;
        /* a(m+1) */
        W x = 0;
        for (idx_t k = 1; k <= m; k++) x += Cc[k] * Aa[m + 1 - k];
        Aa[m + 1] = x / (W)m;
        have = m + 1;
    }
    return (seq_t)Aa[n];
}
