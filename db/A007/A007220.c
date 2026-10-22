/*
 * A007220  Almost-convex polygons of perimeter 2n on square lattice.
 * https://oeis.org/A007220
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. 16*x^3*A/((1-x)*(1-4*x)^(5/2)) +
 *            4*x^3*B/((1-x)*(1-3*x+x^2)*(1-4*x)^3), A = 1 - 9*x + 25*x^2 -
 *            23*x^3 + 3*x^4, B = -4 + 56*x - 300*x^2 + 773*x^3 - 973*x^4 +
 *            535*x^5 - 90*x^6 + 24*x^7; a(n) = [x^n] of the g.f.
 *            (1-4x)^(-5/2) is computed as binomial(2k,k) divided twice by
 *            (1-4x), and 1/((1-x)(1...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_zero, at, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/at.h"

seq_t a(idx_t n) {
    if (n < 6) return 0;
    long long m = (long long)n - 3;
    idx_t len = (idx_t)(m + 1);
    seq_t *e = alloc_zero(len);
    seq_t *s = alloc_zero(len);
    seq_t *d = alloc_zero(len);
    long long k;

    /* e[k] = binomial(2k,k) (modulo the word size in fixed-width builds) */
    if (seq_is_fixed_width()) {
        idx_t rlen = (idx_t)(m + 2);
        seq_t *r = alloc_zero(rlen);
        r[0] = 1;
        e[0] = 1;
        for (long long j = 1; j <= 2 * m; j++) {
            long long top = j < m ? j : m;
            for (long long i = top; i >= 1; i--) r[i] += r[i - 1];
            if ((j & 1) == 0) e[j / 2] = r[j / 2];
        }
        release(r, rlen);
    } else {
        seq_t b = 1;
        e[0] = 1;
        for (k = 1; k <= m; k++) {
            b = b * (seq_t)(2 * (2 * k - 1));
            b = b / (seq_t)k;
            e[k] = b;
        }
    }

    /* divide by (1-4x) twice: (1-4x)^(-5/2), then by (1-x): partial sums */
    for (k = 1; k <= m; k++) e[k] += (seq_t)4 * e[k - 1];
    for (k = 1; k <= m; k++) e[k] += (seq_t)4 * e[k - 1];
    {
        seq_t acc = 0;
        for (k = 0; k <= m; k++) {
            acc += e[k];
            s[k] = acc;
        }
    }

    /* 1/(1 - 16x + 100x^2 - 305x^3 + 460x^4 - 304x^5 + 64x^6) */
    for (k = 0; k <= m; k++) {
        if (k == 0) { d[0] = 1; continue; }
        seq_t pos = (seq_t)16 * at(d, k - 1) + (seq_t)305 * at(d, k - 3) + (seq_t)304 * at(d, k - 5);
        seq_t neg = (seq_t)100 * at(d, k - 2) + (seq_t)460 * at(d, k - 4) + (seq_t)64 * at(d, k - 6);
        d[k] = pos - neg;
    }

    seq_t pos1 = at(s, m) + (seq_t)25 * at(s, m - 2) + (seq_t)3 * at(s, m - 4);
    seq_t neg1 = (seq_t)9 * at(s, m - 1) + (seq_t)23 * at(s, m - 3);
    seq_t pos2 = (seq_t)56 * at(d, m - 1) + (seq_t)773 * at(d, m - 3)
               + (seq_t)535 * at(d, m - 5) + (seq_t)24 * at(d, m - 7);
    seq_t neg2 = (seq_t)4 * at(d, m) + (seq_t)300 * at(d, m - 2)
               + (seq_t)973 * at(d, m - 4) + (seq_t)90 * at(d, m - 6);

    seq_t P = (seq_t)16 * pos1 + (seq_t)4 * pos2;
    seq_t N = (seq_t)16 * neg1 + (seq_t)4 * neg2;
    seq_t res = P - N;

    release(e, len);
    release(s, len);
    release(d, len);
    return res;
}
