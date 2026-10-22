/*
 * A002729  Number of equivalence classes of binary sequences of period n.
 * https://oeis.org/A002729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/(n*phi(n))) * Sum_{k mod n, gcd(k,n)=1} Sum_{t mod n}
 *            2^C(k,t), where C(k,t) is the number of cycles of x -> k*x+t on
 *            Z_n (Burnside over the affine group); a(0) = 1. C(k,t) depends
 *            only on gcd(t mod d, d), d = gcd(k-1,n)
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 41, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   a2729_gcd, a2729_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2729_gcd.h"
#include "../utils/a2729_phi.h"

seq_t a(idx_t n) {
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
        if (a2729_gcd(k, p) != 1) continue;
        ph++;
        d = a2729_gcd((k + p - 1) % p, p);
        for (e = 1; e <= d; e++) {
            uint64_t mult, t, cyc = 0;
            if (d % e) continue;
            mult = a2729_phi(d / e) * (p / d);
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
    /* Horner evaluation of Sum cnt[c]*2^c, kept as Q*N + R to divide exactly */
    for (i = (idx_t)p + 1; i-- > 0;) {
        R = 2 * R + cnt[i];
        Q = Q * 2 + (seq_t)(R / N);
        R %= N;
    }

    free(cnt);
    free(mark);
    return Q;
}
