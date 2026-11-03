/*
 * A005199  a(n) = Sum_t t*F(n,t), where F(n,t) is the number of forests with
 *          n (unlabeled) nodes and exactly t trees, all of which are planted
 *          (that is, rooted trees in which the root has degree 1).
 * https://oeis.org/A005199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n} D(m)*E(n-m), where p(k) = A000081(k-1) for k
 *            >= 2 (planted trees with k nodes), p(1) = 0, D(m) = Sum_{d|m}
 *            p(d), and E(x) = Sum E(n) x^n = Product_{k>=2} 1/(1-x^k)^p(k)
 *            (Euler transform: E(0) = 1, E(n) = (1/n) Sum_{m=1..n} (Sum_{d|m}
 *            d*p(d)) E(n-m)); this is d/dy at y=1 of Pro...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 47, u128 n <= 88,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 118 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static unsigned __int128 *grow_u128(unsigned __int128 *old, idx_t oldlen, idx_t newlen) {
    unsigned __int128 *v = (unsigned __int128 *)calloc((size_t)newlen, sizeof(unsigned __int128));
    if (old) {
        memcpy(v, old, (size_t)oldlen * sizeof(unsigned __int128));
        free(old);
    }
    return v;
}

#define DEFINE_IMPL(S, T, GROW)                                              \
static T *r_##S, *cr_##S, *p_##S, *ce_##S, *D_##S, *E_##S;                   \
static idx_t cap_##S = 0, done_##S = 0;                                      \
static void extend_##S(idx_t N) {                                            \
    if (N <= done_##S) return;                                               \
    if (N + 1 > cap_##S) {                                                   \
        idx_t nc = cap_##S ? cap_##S : 64;                                   \
        bool first = (cap_##S == 0);                                         \
        while (nc < N + 1) nc *= 2;                                          \
        r_##S = GROW(r_##S, cap_##S, nc);                                    \
        cr_##S = GROW(cr_##S, cap_##S, nc);                                  \
        p_##S = GROW(p_##S, cap_##S, nc);                                    \
        ce_##S = GROW(ce_##S, cap_##S, nc);                                  \
        D_##S = GROW(D_##S, cap_##S, nc);                                    \
        E_##S = GROW(E_##S, cap_##S, nc);                                    \
        if (first) E_##S[0] = 1;                                             \
        cap_##S = nc;                                                        \
    }                                                                        \
    for (idx_t i = done_##S + 1; i <= N; i++) {                              \
        T s;                                                                 \
        if (i == 1) {                                                        \
            r_##S[1] = 1;                                                    \
        } else {                                                             \
            s = 0;                                                           \
            for (idx_t k = 1; k < i; k++) s += cr_##S[k] * r_##S[i - k];     \
            r_##S[i] = s / (T)(i - 1);                                       \
        }                                                                    \
        if (i >= 2) p_##S[i] = r_##S[i - 1];                                 \
        else p_##S[i] = 0;                                                   \
        cr_##S[i] = 0; ce_##S[i] = 0; D_##S[i] = 0;                          \
        for (idx_t d = 1; d * d <= i; d++) {                                 \
            if (i % d == 0) {                                                \
                idx_t e = i / d;                                             \
                cr_##S[i] += (T)d * r_##S[d];                                \
                ce_##S[i] += (T)d * p_##S[d];                                \
                D_##S[i] += p_##S[d];                                        \
                if (e != d) {                                                \
                    cr_##S[i] += (T)e * r_##S[e];                            \
                    ce_##S[i] += (T)e * p_##S[e];                            \
                    D_##S[i] += p_##S[e];                                    \
                }                                                            \
            }                                                                \
        }                                                                    \
        s = 0;                                                               \
        for (idx_t m = 1; m <= i; m++) s += ce_##S[m] * E_##S[i - m];        \
        E_##S[i] = s / (T)i;                                                 \
    }                                                                        \
    done_##S = N;                                                            \
}                                                                            \
static T calc_##S(idx_t n) {                                                 \
    extend_##S(n);                                                           \
    T s = 0;                                                                 \
    for (idx_t m = 1; m <= n; m++) s += D_##S[m] * E_##S[n - m];             \
    return s;                                                                \
}

DEFINE_IMPL(big, seq_t, grow_zero)
DEFINE_IMPL(u, unsigned __int128, grow_u128)

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (seq_is_fixed_width()) return u128_to_seq(calc_u(n));
    return calc_big(n);
}
