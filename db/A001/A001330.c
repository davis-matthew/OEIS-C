/*
 * A001330  Number of n-element algebras with 2 binary operations.
 * https://oeis.org/A001330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n}
 *            fixA[s_1,s_2,...]/(1^s_1*s_1!*2^s_2*s_2!*...), fixA[s_1,s_2,...]
 *            = Product_{i,j>=1} (Sum_{d|lcm(i,j)}
 *            d*s_d)^(2*gcd(i,j)*s_i*s_j). Computed as (Sum over cycle types
 *            of fixA * n!/z) / n!
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a1330_fixed, a1330_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a1330_fixed.h"
#include "../utils/a1330_gcd.h"

#define A1330_DEF(T, SUF)                                                     \
static T a1330_pw_##SUF(T b, idx_t e) {                                       \
    T r = 1;                                                                  \
    while (e > 0) {                                                           \
        if (e & 1) r *= b;                                                    \
        e >>= 1;                                                              \
        if (e) b *= b;                                                        \
    }                                                                         \
    return r;                                                                 \
}                                                                             \
static void a1330_rec_##SUF(idx_t rem, idx_t maxp, idx_t *s, idx_t *act,      \
                            int nact, T nfact, T *total) {                    \
    if (rem == 0) {                                                           \
        T fix = 1, den = 1;                                                   \
        int x, y, z;                                                          \
        for (x = 0; x < nact; x++) {                                          \
            idx_t i = act[x], k;                                              \
            den *= a1330_pw_##SUF((T)i, s[i]);                                \
            for (k = 2; k <= s[i]; k++) den *= (T)k;                          \
            for (y = 0; y < nact; y++) {                                      \
                idx_t j = act[y];                                             \
                idx_t g = a1330_gcd(i, j);                                    \
                idx_t l = i / g * j;                                          \
                idx_t base = 0;                                               \
                for (z = 0; z < nact; z++) {                                  \
                    idx_t d = act[z];                                         \
                    if (l % d == 0) base += d * s[d];                         \
                }                                                             \
                fix *= a1330_pw_##SUF((T)base, 2 * g * s[i] * s[j]);          \
            }                                                                 \
        }                                                                     \
        *total += fix * (nfact / den);                                        \
        return;                                                               \
    }                                                                         \
    {                                                                         \
        idx_t p = maxp < rem ? maxp : rem;                                    \
        for (; p >= 1; p--) {                                                 \
            idx_t c;                                                          \
            for (c = 1; c * p <= rem; c++) {                                  \
                s[p] = c;                                                     \
                act[nact] = p;                                                \
                a1330_rec_##SUF(rem - c * p, p - 1, s, act, nact + 1,         \
                                nfact, total);                                \
            }                                                                 \
            s[p] = 0;                                                         \
        }                                                                     \
    }                                                                         \
}

A1330_DEF(unsigned __int128, u)
A1330_DEF(seq_t, s)

seq_t a(idx_t n) {
    idx_t *s, *act, k;
    seq_t res = 0;
    if (n == 0) return 1;
    s = (idx_t *)calloc(n + 1, sizeof(idx_t));
    act = (idx_t *)calloc(n + 1, sizeof(idx_t));
    if (!s || !act) { free(s); free(act); return 0; }
    if (a1330_fixed()) {
        unsigned __int128 nf = 1, tot = 0;
        int c;
        for (k = 2; k <= n; k++) nf *= (unsigned __int128)k;
        a1330_rec_u(n, n, s, act, 0, nf, &tot);
        tot /= nf;
        for (c = 7; c >= 0; c--) {
            res = res * 65536 + (seq_t)(uint64_t)((tot >> (16 * c)) & 0xFFFF);
        }
    } else {
        seq_t nf = 1, tot = 0;
        for (k = 2; k <= n; k++) nf *= (seq_t)k;
        a1330_rec_s(n, n, s, act, 0, nf, &tot);
        res = tot / nf;
    }
    free(s);
    free(act);
    return res;
}
