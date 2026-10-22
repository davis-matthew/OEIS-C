/*
 * A003649  Class number of real quadratic field Q(sqrt f), where f is the
 *          n-th squarefree number A005117(n).
 * https://oeis.org/A003649
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(Q(sqrt f)), f = A005117(n), computed by counting
 *            classes of reduced indefinite binary quadratic forms of the
 *            field discriminant D (D = f if f == 1 mod 4, else 4f): reduced
 *            forms are grouped into cycles under the rho operator, and each
 *            form is also merged with (-a,b,-c), which gives the w...
 * Range:     u8 n <= 6083, u16 n <= 6083, u32 n <= 6083, u64 n <= 6083,
 *            u128 n <= 6083, big any n
 * Helpers:   sqf, uf_find, uf_union
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sqf.h"
#include "../utils/uf_find.h"
#include "../utils/uf_union.h"

static uint64_t nth_squarefree(idx_t n) {
    idx_t count = 0;
    uint64_t m = 0;
    while (count < n) {
        m++;
        if (sqf(m)) count++;
    }
    return m;
}

static long long class_number(long long f) {
    long long D = (f % 4 == 1) ? f : 4 * f;
    long long s = 0;
    while ((s + 1) * (s + 1) <= D) s++;
    long long W = s + 1;               /* b in 0..s */
    long long H = 2 * s + 1;           /* a in -s..s */
    long long *idx = (long long *)malloc(sizeof(long long) * H * W);
    for (long long i = 0; i < H * W; i++) idx[i] = -1;
    long long cap = 1024, cnt = 0;
    long long *fa = (long long *)malloc(sizeof(long long) * cap);
    long long *fb = (long long *)malloc(sizeof(long long) * cap);
    for (long long b = 1; b <= s; b++) {
        if (((b ^ D) & 1) != 0) continue;
        long long N = (D - b * b) / 4;
        for (long long m = 1; m <= N; m++) {
            if (N % m) continue;
            if (!(s - b < 2 * m && 2 * m <= s + b)) continue;
            for (int sg = -1; sg <= 1; sg += 2) {
                long long av = sg * m;
                if (cnt == cap) {
                    cap *= 2;
                    fa = (long long *)realloc(fa, sizeof(long long) * cap);
                    fb = (long long *)realloc(fb, sizeof(long long) * cap);
                }
                fa[cnt] = av; fb[cnt] = b;
                idx[(av + s) * W + b] = cnt;
                cnt++;
            }
        }
    }
    long long *par = (long long *)malloc(sizeof(long long) * (cnt > 0 ? cnt : 1));
    for (long long i = 0; i < cnt; i++) par[i] = i;
    for (long long i = 0; i < cnt; i++) {
        long long av = fa[i], b = fb[i];
        long long c = (b * b - D) / (4 * av);
        long long ac = c < 0 ? -c : c;
        long long mod = 2 * ac;
        /* b' == -b mod 2|c|, s - 2|c| < b' <= s */
        long long r = ((-b) % mod + mod) % mod;
        long long bp = s - (((s - r) % mod) + mod) % mod;
        long long j = idx[(c + s) * W + bp];
        if (j >= 0) uf_union(par, i, j);
        long long k = idx[(-av + s) * W + b];
        if (k >= 0) uf_union(par, i, k);
    }
    long long h = 0;
    for (long long i = 0; i < cnt; i++)
        if (uf_find(par, i) == i) h++;
    free(idx); free(fa); free(fb); free(par);
    return h;
}

seq_t a(idx_t n) {
    uint64_t f = nth_squarefree(n);
    long long h = class_number((long long)f);
    seq_t r = 0;
    r += (seq_t)(uint64_t)h;
    return r;
}
