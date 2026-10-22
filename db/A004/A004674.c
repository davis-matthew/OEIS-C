/*
 * A004674  Theta series of extremal even unimodular lattice in dimension 64.
 * https://oeis.org/A004674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. E4^8 - 1920*E4^5*Delta + 627840*E4^2*Delta^2, evaluated as
 *            a(n) = [q^n] (E8^2*Y + 627840*E8*Delta^2), where E8 = E4^2 = 1 +
 *            480*Sum sigma_7(k) q^k, E8^2 = E4^4 = 1 + (16320*Sum sigma_15(k)
 *            q^k + 3456000*E4*Delta)/3617, Y = E8^2 - 1920*E4*Delta, Delta =
 *            q*Prod_{k>=1} (1-q^k)^24 (Delta and Del...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_zero, from_i128, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"

static idx_t cap = 0;
static seq_t *E8s = NULL, *E16s = NULL, *Ys = NULL, *D2 = NULL;

static void build(idx_t N) {
    idx_t i, j, k, m, np = 0;
    __int128 *h = (__int128 *)malloc(N * sizeof(__int128));
    __int128 *tau = (__int128 *)malloc(N * sizeof(__int128));
    __int128 *e4 = (__int128 *)malloc(N * sizeof(__int128));
    __int128 *e8 = (__int128 *)malloc(N * sizeof(__int128));
    __int128 *e4d = (__int128 *)malloc(N * sizeof(__int128));
    idx_t *pp = (idx_t *)malloc((N + 2) * sizeof(idx_t));
    int *ps = (int *)malloc((N + 2) * sizeof(int));

    if (cap > 0) {
        release(E8s, cap); release(E16s, cap); release(Ys, cap); release(D2, cap);
    }
    E8s = alloc_zero(N); E16s = alloc_zero(N); Ys = alloc_zero(N); D2 = alloc_zero(N);

    /* generalized pentagonal numbers with signs of Prod (1-q^k) */
    for (k = 1;; k++) {
        idx_t p1 = k * (3 * k - 1) / 2, p2 = k * (3 * k + 1) / 2;
        int s = (k & 1) ? -1 : 1;
        if (p1 >= N) break;
        pp[np] = p1; ps[np++] = s;
        if (p2 < N) { pp[np] = p2; ps[np++] = s; }
    }

    /* sigma_3 and sigma_7 sieves */
    for (i = 0; i < N; i++) { e4[i] = 0; e8[i] = 0; }
    for (i = 1; i < N; i++) {
        __int128 d3 = (__int128)i * i * i;
        __int128 d7 = d3 * d3 * i;
        for (m = i; m < N; m += i) { e4[m] += d3; e8[m] += d7; }
    }
    for (i = 1; i < N; i++) { e4[i] *= 240; e8[i] *= 480; }
    e4[0] = 1; e8[0] = 1;

    /* tau via Prod (1-q^k)^24 */
    h[0] = 1;
    for (m = 1; m < N; m++) {
        __int128 s = 0;
        for (i = 0; i < np && pp[i] <= m; i++)
            s += (__int128)(25 * (long long)pp[i] - (long long)m) * ps[i] * h[m - pp[i]];
        h[m] = s / (__int128)m;
    }
    tau[0] = 0;
    for (i = 1; i < N; i++) tau[i] = h[i - 1];

    /* E4*Delta */
    for (i = 0; i < N; i++) {
        __int128 s = 0;
        for (j = 0; j < i; j++) s += e4[j] * tau[i - j];
        e4d[i] = s;
    }

    for (i = 0; i < N; i++) E8s[i] = from_i128(e8[i]);

    if (seq_is_fixed_width()) {
        seq_t *T = alloc_zero(N);
        for (i = 0; i < N; i++) T[i] = from_i128(tau[i]);
        for (i = 0; i < N; i++) {
            seq_t s = 0, t = 0;
            for (j = 0; j <= i; j++) {
                s += E8s[j] * E8s[i - j];
                t += T[j] * T[i - j];
            }
            E16s[i] = s;
            D2[i] = t;
        }
        release(T, N);
    } else {
        /* sigma_15 sieve, then E4^4 closed form */
        for (i = 1; i < N; i++) {
            seq_t p = 1;
            for (j = 0; j < 15; j++) p *= (seq_t)i;
            for (m = i; m < N; m += i) E16s[m] += p;
        }
        E16s[0] = 1;
        for (i = 1; i < N; i++)
            E16s[i] = (E16s[i] * 16320 + from_i128(e4d[i]) * 3456000) / 3617;
        /* Delta^2 = q^2 * Prod (1-q^k)^48 */
        if (N > 2) {
            D2[2] = 1;
            for (m = 1; m + 2 < N; m++) {
                seq_t s = 0;
                for (i = 0; i < np && pp[i] <= m; i++)
                    s += from_i128((__int128)(49 * (long long)pp[i] - (long long)m) * ps[i])
                         * D2[m - pp[i] + 2];
                D2[m + 2] = s / (seq_t)m;
            }
        }
    }

    for (i = 0; i < N; i++) Ys[i] = E16s[i] - from_i128(e4d[i]) * 1920;

    free(h); free(tau); free(e4); free(e8); free(e4d); free(pp); free(ps);
    cap = N;
}

seq_t a(idx_t n) {
    seq_t s = 0, t = 0;
    idx_t j;
    if (n >= cap) {
        idx_t N = n + 1 + n / 2;
        if (N < 64) N = 64;
        build(N);
    }
    for (j = 0; j <= n; j++) s += E16s[j] * Ys[n - j];
    for (j = 0; j + 2 <= n; j++) t += E8s[j] * D2[n - j];
    return s + t * 627840;
}
