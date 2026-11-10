/*
 * A062163  Duplicate of A002729.
 * https://oeis.org/A062163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (1/(n*phi(n))) * Sum_{k mod n, gcd(k,n)=1} Sum_{t mod n}
 *            2^C(k,t), a(0) = 1; duplicate of A002729 (Burnside over the
 *            affine group)
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-07)
 * Helpers:   gcd_u64, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/totient_u64.h"

static seq_t A002729_a(idx_t n) {
    uint64_t p, k, e, x, y, ph = 0, N, R = 0;
    uint64_t *cnt;
    unsigned *mark;
    unsigned stamp = 0;
    seq_t Q = 0;
    idx_t i;

    if (n == 0) return 1;
    p = (uint64_t)n;
    cnt = (uint64_t *)calloc((size_t)(p + 1), sizeof(uint64_t));
    mark = (unsigned *)calloc((size_t)p, sizeof(unsigned));

    for (k = 0; k < p; k++) {
        uint64_t d;
        if (gcd_u64(k, p) != 1) continue;
        ph++;
        d = gcd_u64((k + p - 1) % p, p);
        for (e = 1; e <= d; e++) {
            uint64_t mult, t, cyc = 0;
            if (d % e) continue;
            mult = totient_u64(d / e) * (p / d);
            t = e % p;
            stamp++;
            for (x = 0; x < p; x++) {
                if (mark[x] != stamp) {
                    cyc++;
                    y = x;
                    while (mark[y] != stamp) {
                        mark[y] = stamp;
                        y = (k * y + t) % p;
                    }
                }
            }
            cnt[cyc] += mult;
        }
    }

    N = p * ph;
    for (i = (idx_t)p + 1; i-- > 0;) {
        R = 2 * R + cnt[i];
        Q = Q * 2 + (seq_t)(R / N);
        R %= N;
    }

    free(cnt);
    free(mark);
    return Q;
}

seq_t a(idx_t n) {
    return A002729_a(n);
}
