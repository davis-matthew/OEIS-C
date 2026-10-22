/*
 * A003646  Class number of binary quadratic forms with fundamental
 *          discriminant A003658(n),n>=2.
 * https://oeis.org/A003646
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = narrow class number h+(D) of D = A003658(n), the n-th
 *            positive fundamental discriminant (found by a search); h+(D) is
 *            the number of cycles of reduced indefinite forms (a,b,c),
 *            b^2-4ac = D, under the rho reduction operator
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   isqrt_ll, reduced
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_ll.h"
#include "../utils/reduced.h"

static bool sqfree_ll(long long m) {
    for (long long p = 2; p * p <= m; p++)
        if (m % (p * p) == 0) return false;
    return true;
}

static bool is_fund_pos(long long d) {
    if (d == 1) return true;
    if (d % 4 == 1) return sqfree_ll(d);
    if (d % 4 == 0) {
        long long m = d / 4;
        return (m % 4 == 2 || m % 4 == 3) && sqfree_ll(m);
    }
    return false;
}

seq_t a(idx_t n) {
    long long D = 0, cnt = 0, s, b, nf = 0, cap = 64, i, cycles = 0;
    long long *fa, *fb;
    char *vis;
    while (cnt < (long long)n) {
        D++;
        if (is_fund_pos(D)) cnt++;
    }
    if (D == 1) return (seq_t)1;
    s = isqrt_ll(D);
    fa = (long long *)malloc(sizeof(long long) * cap);
    fb = (long long *)malloc(sizeof(long long) * cap);
    for (b = (D % 2); b <= s; b += 2) {
        long long m, d;
        if (b <= 0 || (D - b * b) % 4 != 0) continue;
        m = (D - b * b) / 4;
        for (d = 1; d <= m; d++) {
            int sg;
            if (m % d) continue;
            for (sg = 0; sg < 2; sg++) {
                long long aa = sg ? -d : d;
                if (reduced(aa, b, D)) {
                    if (nf == cap) {
                        cap *= 2;
                        fa = (long long *)realloc(fa, sizeof(long long) * cap);
                        fb = (long long *)realloc(fb, sizeof(long long) * cap);
                    }
                    fa[nf] = aa; fb[nf] = b; nf++;
                }
            }
        }
    }
    vis = (char *)calloc((size_t)(nf + 1), 1);
    for (i = 0; i < nf; i++) {
        long long ca, cb, j;
        if (vis[i]) continue;
        cycles++;
        ca = fa[i]; cb = fb[i];
        j = i;
        while (!vis[j]) {
            long long cc, C, bp, r, k;
            vis[j] = 1;
            cc = (cb * cb - D) / (4 * ca);
            C = 2 * (cc < 0 ? -cc : cc);
            r = ((s + cb) % C + C) % C;
            bp = s - r;
            ca = cc; cb = bp;
            for (k = 0; k < nf; k++)
                if (fa[k] == ca && fb[k] == cb) break;
            if (k == nf) break;
            j = k;
        }
    }
    free(fa); free(fb); free(vis);
    return (seq_t)cycles;
}
