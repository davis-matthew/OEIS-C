/*
 * A006691  Normalized number of connected (n+1)-state finite automata with 2
 *          inputs.
 * https://oeis.org/A006691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(n+1)/n!, where S(p) = number of labeled strongly
 *            connected p-state automata with 2 inputs (Liskovets/Robinson):
 *            for each T: g_T(m) = T^(2m) - Sum_{k=1..m-1}
 *            C(m,k)*g_T(k)*(T-k)^(2(m-k)), h(T) = g_T(T); S(1) = 1 = h(1),
 *            S(n) = h(n) + Sum_{k=1..n-1} C(n-1,k-1)*S(k)*h(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, mpow_p, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/mpow_p.h"

/* ---------- modular path (fixed-width builds) ---------- */

static bool small_isprime(uint64_t x) {
    uint64_t d;
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t solve_mod(idx_t N, uint64_t p) {
    uint64_t *h = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *S = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *g = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *C = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *P = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    idx_t T, m, k, n;
    uint64_t res, fact;

    for (T = 1; T <= N; T++) {
        uint64_t t2 = (uint64_t)(T % p) * (T % p) % p, tp = 1;
        for (k = 0; k <= T; k++) { g[k] = 0; C[k] = 0; P[k] = 0; }
        C[0] = 1;
        for (m = 1; m <= T; m++) {
            uint64_t v;
            for (k = m; k >= 1; k--) C[k] = (C[k] + C[k - 1]) % p;
            for (k = 1; k + 1 < m; k++) {
                uint64_t b = (uint64_t)((T - k) % p);
                P[k] = P[k] * (b * b % p) % p;
            }
            if (m >= 2) {
                uint64_t b = (uint64_t)((T - m + 1) % p);
                P[m - 1] = b * b % p;
            }
            tp = tp * t2 % p;
            v = tp;
            for (k = 1; k < m; k++) {
                uint64_t t = C[k] * g[k] % p * P[k] % p;
                v = (v + p - t) % p;
            }
            g[m] = v;
        }
        h[T] = g[T];
    }

    /* C reused as Pascal row n-1 */
    for (k = 0; k <= N; k++) C[k] = 0;
    C[0] = 1;
    for (n = 1; n <= N; n++) {
        uint64_t v;
        if (n >= 2)
            for (k = n - 1; k >= 1; k--) C[k] = (C[k] + C[k - 1]) % p;
        v = h[n];
        for (k = 1; k < n; k++)
            v = (v + C[k - 1] * S[k] % p * h[n - k]) % p;
        S[n] = v;
    }

    fact = 1;
    for (k = 1; k < N; k++) fact = fact * (uint64_t)(k % p) % p;
    res = S[N] * mpow_p(fact, p - 2, p) % p;

    free(h); free(S); free(g); free(C); free(P);
    return res;
}

static seq_t a_fixed(idx_t n) {
    uint64_t primes[8], resid[8], dig[8];
    int r, i, j, cnt = 0;
    int bits = (int)(sizeof(seq_t) * 8);
    uint64_t cand = 2147483647ULL;
    seq_t res = 0, M = 1;

    if (n > 40) return 0; /* value far exceeds any fixed width here */
    r = bits / 31 + 1;
    while (cnt < r) {
        if (small_isprime(cand)) primes[cnt++] = cand;
        cand--;
    }
    for (i = 0; i < r; i++) resid[i] = solve_mod(n + 1, primes[i]);
    /* Garner */
    for (i = 0; i < r; i++) {
        uint64_t p = primes[i], t = 0, mult = 1;
        for (j = 0; j < i; j++) {
            t = (t + dig[j] % p * mult) % p;
            mult = mult * (primes[j] % p) % p;
        }
        dig[i] = (resid[i] % p + p - t) % p * mpow_p(mult, p - 2, p) % p;
    }
    for (i = 0; i < r; i++) {
        res += (seq_t)dig[i] * M;
        M *= (seq_t)primes[i];
    }
    return res;
}

/* ---------- exact path (arbitrary precision) ---------- */
/* signed values stored as (pos, neg) pairs of nonnegative seq_t */

static seq_t *Hp = NULL, *Hn = NULL, *Sv = NULL;
static idx_t have = 0, cap = 0;

static void diag_big(idx_t T, seq_t *outp, seq_t *outn) {
    seq_t *gp = alloc_zero(T + 1);
    seq_t *gn = alloc_zero(T + 1);
    seq_t *C = alloc_zero(T + 1);
    seq_t *P = alloc_zero(T + 1);
    seq_t t2 = (seq_t)T * (seq_t)T;
    seq_t tp = 1;
    idx_t m, k;

    C[0] = 1;
    for (m = 1; m <= T; m++) {
        seq_t sp = 0, sn = 0, A, B;
        for (k = m; k >= 1; k--) C[k] += C[k - 1];
        for (k = 1; k + 1 < m; k++) P[k] *= (seq_t)(T - k) * (seq_t)(T - k);
        if (m >= 2) P[m - 1] = (seq_t)(T - m + 1) * (seq_t)(T - m + 1);
        tp *= t2;
        for (k = 1; k < m; k++) {
            seq_t c = C[k] * P[k];
            sp += c * gp[k];
            sn += c * gn[k];
        }
        A = tp + sn;
        B = sp;
        if (A >= B) { gp[m] = A - B; gn[m] = 0; }
        else { gp[m] = 0; gn[m] = B - A; }
    }
    *outp = gp[T];
    *outn = gn[T];
    release(gp, T + 1);
    release(gn, T + 1);
    release(C, T + 1);
    release(P, T + 1);
}

static void ensure_big(idx_t N) {
    idx_t T, k;
    if (N + 1 > cap) {
        idx_t nc = cap * 2;
        seq_t *np, *nn, *ns;
        if (nc < N + 1) nc = N + 1;
        np = alloc_zero(nc);
        nn = alloc_zero(nc);
        ns = alloc_zero(nc);
        for (k = 0; k <= have && k < cap; k++) {
            np[k] = Hp[k]; nn[k] = Hn[k]; ns[k] = Sv[k];
        }
        if (Hp) { release(Hp, cap); release(Hn, cap); release(Sv, cap); }
        Hp = np; Hn = nn; Sv = ns;
        cap = nc;
    }
    for (T = have + 1; T <= N; T++) {
        seq_t ps = 0, ns2 = 0, c = 1, A, B;
        diag_big(T, &Hp[T], &Hn[T]);
        for (k = 1; k < T; k++) {
            /* c = C(T-1, k-1) */
            seq_t q = c * Sv[k];
            ps += q * Hp[T - k];
            ns2 += q * Hn[T - k];
            c = c * (seq_t)(T - k) / (seq_t)k;
        }
        A = Hp[T] + ps;
        B = Hn[T] + ns2;
        Sv[T] = A - B;
        have = T;
    }
}

seq_t a(idx_t n) {
    seq_t fact = 1;
    idx_t i;
    if (n == 0) return 0;
    if (seq_is_fixed_width()) return a_fixed(n);
    ensure_big(n + 1);
    for (i = 2; i <= n; i++) fact *= (seq_t)i;
    return Sv[n + 1] / fact;
}
