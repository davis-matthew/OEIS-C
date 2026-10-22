/*
 * A003456  Number of nonequivalent dissections of an n-gon by nonintersecting
 *          diagonals rooted at a cell up to rotation and reflection.
 * https://oeis.org/A003456
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( 4*Sum_{d|n} phi(d)*P(n/d) + 4n*Sum_{i=1..n-1}
 *            S(i)*H(n-i) + 2n*Sum_{i=1..n-1} (g(i)+S2(i))*H(n-i) - 4n*G(n) -
 *            2n*(Q(n)+g(n)) ) / (8n), where G(1)=1, G(m)=2*Q(m)-G(m-1),
 *            Q(m)=Sum_{i=1..m-1} G(i)*G(m-i) (little Schroeder numbers), C =
 *            1/(1-G) as series, P(m)=Sum_{i=1..m} i*G(i)*C(m-i), g(m...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 31, u128 n <= 56,
 *            big any n
 * Helpers:   A_grow, a3456_fixed, a3456_phi, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/A_grow.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a3456_fixed.h"
#include "../utils/a3456_phi.h"

/* ---------- arbitrary precision path (exact, cached) ---------- */
static seq_t *bG, *bQ, *bC, *bS, *bS2, *bH;
static idx_t bcap = 0;

static seq_t a3456_bg(idx_t m)
{
    if (m > 0 && m % 2 == 0) return bG[m / 2];
    seq_t z = 0;
    return z;
}

static void a3456_bgrow(idx_t L)
{
    idx_t nl, m, i;
    if (L <= bcap) return;
    nl = L;
    if (nl < 2 * bcap) nl = 2 * bcap;
    bG = A_grow(bG, bcap, nl);
    bQ = A_grow(bQ, bcap, nl);
    bC = A_grow(bC, bcap, nl);
    bS = A_grow(bS, bcap, nl);
    bS2 = A_grow(bS2, bcap, nl);
    bH = A_grow(bH, bcap, nl);
    for (m = bcap; m < nl; m++) {
        seq_t q = 0, c = 0, s = 0, s2 = 0, h = 0;
        if (m == 0) {
            bG[0] = 0; bQ[0] = 0; bC[0] = 1; bS[0] = 0; bS2[0] = 0; bH[0] = 0;
            continue;
        }
        for (i = 1; i < m; i++) q += bG[i] * bG[m - i];
        bQ[m] = q;
        if (m == 1) bG[m] = 1;
        else bG[m] = q * 2 - bG[m - 1];
        for (i = 1; i <= m; i++) c += bG[i] * bC[m - i];
        bC[m] = c;
        if (m == 1) s += 1;
        s += a3456_bg(m);
        for (i = 2; i < m; i += 2) s += a3456_bg(i) * bS[m - i] * 2;
        s -= a3456_bg(m - 1);
        bS[m] = s;
        for (i = 0; i <= m; i++) s2 += bS[i] * bS[m - i];
        bS2[m] = s2;
        h += a3456_bg(m);
        for (i = 2; i < m; i += 2) h += a3456_bg(i) * bH[m - i];
        bH[m] = h;
    }
    bcap = nl;
}

static seq_t a3456_big(idx_t n)
{
    idx_t d, i;
    seq_t pos = 0, neg = 0, t, u;
    a3456_bgrow(n + 1);
    for (d = 1; d <= n; d++)
        if (n % d == 0) {
            idx_t m = n / d;
            seq_t p = 0;
            for (i = 1; i <= m; i++) p += (seq_t)i * bG[i] * bC[m - i];
            pos += p * a3456_phi(d) * 4;
        }
    t = 0;
    for (i = 1; i < n; i++) t += bS[i] * bH[n - i];
    pos += t * n * 4;
    u = 0;
    for (i = 1; i < n; i++) u += (a3456_bg(i) + bS2[i]) * bH[n - i];
    pos += u * n * 2;
    neg += bG[n] * n * 4;
    neg += (bQ[n] + a3456_bg(n)) * n * 2;
    return (pos - neg) / ((seq_t)n * 8);
}

/* ---------- fixed width path: exact mod 2^192, then exact division ---------- */
typedef struct { uint64_t w[3]; } a3456_u;

static a3456_u a3456_U(uint64_t v)
{
    a3456_u r;
    r.w[0] = v; r.w[1] = 0; r.w[2] = 0;
    return r;
}

static a3456_u a3456_add(a3456_u a, a3456_u b)
{
    a3456_u r;
    unsigned __int128 t = 0;
    int i;
    for (i = 0; i < 3; i++) {
        t += (unsigned __int128)a.w[i] + b.w[i];
        r.w[i] = (uint64_t)t;
        t >>= 64;
    }
    return r;
}

static a3456_u a3456_sub(a3456_u a, a3456_u b)
{
    a3456_u r;
    uint64_t br = 0;
    int i;
    for (i = 0; i < 3; i++) {
        unsigned __int128 t = (unsigned __int128)a.w[i] - b.w[i] - br;
        r.w[i] = (uint64_t)t;
        br = (t >> 64) ? 1 : 0;
    }
    return r;
}

static a3456_u a3456_mul(a3456_u a, a3456_u b)
{
    a3456_u r;
    uint64_t z[3] = {0, 0, 0};
    int i, j;
    for (i = 0; i < 3; i++) {
        uint64_t carry = 0;
        for (j = 0; i + j < 3; j++) {
            unsigned __int128 t = (unsigned __int128)a.w[i] * b.w[j] + z[i + j] + carry;
            z[i + j] = (uint64_t)t;
            carry = (uint64_t)(t >> 64);
        }
    }
    r.w[0] = z[0]; r.w[1] = z[1]; r.w[2] = z[2];
    return r;
}

static seq_t a3456_fix(idx_t n)
{
    idx_t L = n + 1, m, i, d;
    a3456_u *G, *Q, *C, *S, *S2, *H;
    a3456_u pos = a3456_U(0), neg = a3456_U(0), t, u;
    uint64_t D = 8 * (uint64_t)n, o;
    int e = 0, k;
    unsigned __int128 X, inv;
    uint64_t r0, r1;

    G = (a3456_u *)malloc(L * sizeof(a3456_u));
    Q = (a3456_u *)malloc(L * sizeof(a3456_u));
    C = (a3456_u *)malloc(L * sizeof(a3456_u));
    S = (a3456_u *)malloc(L * sizeof(a3456_u));
    S2 = (a3456_u *)malloc(L * sizeof(a3456_u));
    H = (a3456_u *)malloc(L * sizeof(a3456_u));
#define A3456_g(mm) (((mm) > 0 && (mm) % 2 == 0) ? G[(mm) / 2] : a3456_U(0))
    for (m = 0; m < L; m++) {
        a3456_u q = a3456_U(0), c = a3456_U(0), s = a3456_U(0), s2 = a3456_U(0), h = a3456_U(0);
        if (m == 0) {
            G[0] = Q[0] = S[0] = S2[0] = H[0] = a3456_U(0);
            C[0] = a3456_U(1);
            continue;
        }
        for (i = 1; i < m; i++) q = a3456_add(q, a3456_mul(G[i], G[m - i]));
        Q[m] = q;
        if (m == 1) G[m] = a3456_U(1);
        else G[m] = a3456_sub(a3456_add(q, q), G[m - 1]);
        for (i = 1; i <= m; i++) c = a3456_add(c, a3456_mul(G[i], C[m - i]));
        C[m] = c;
        if (m == 1) s = a3456_U(1);
        s = a3456_add(s, A3456_g(m));
        for (i = 2; i < m; i += 2) s = a3456_add(s, a3456_mul(a3456_mul(A3456_g(i), S[m - i]), a3456_U(2)));
        s = a3456_sub(s, A3456_g(m - 1));
        S[m] = s;
        for (i = 0; i <= m; i++) s2 = a3456_add(s2, a3456_mul(S[i], S[m - i]));
        S2[m] = s2;
        h = A3456_g(m);
        for (i = 2; i < m; i += 2) h = a3456_add(h, a3456_mul(A3456_g(i), H[m - i]));
        H[m] = h;
    }
    for (d = 1; d <= n; d++)
        if (n % d == 0) {
            idx_t mm = n / d;
            a3456_u p = a3456_U(0);
            for (i = 1; i <= mm; i++)
                p = a3456_add(p, a3456_mul(a3456_mul(a3456_U(i), G[i]), C[mm - i]));
            pos = a3456_add(pos, a3456_mul(p, a3456_U(a3456_phi(d) * 4)));
        }
    t = a3456_U(0);
    for (i = 1; i < n; i++) t = a3456_add(t, a3456_mul(S[i], H[n - i]));
    pos = a3456_add(pos, a3456_mul(t, a3456_U(4 * (uint64_t)n)));
    u = a3456_U(0);
    for (i = 1; i < n; i++) u = a3456_add(u, a3456_mul(a3456_add(A3456_g(i), S2[i]), H[n - i]));
    pos = a3456_add(pos, a3456_mul(u, a3456_U(2 * (uint64_t)n)));
    neg = a3456_add(neg, a3456_mul(G[n], a3456_U(4 * (uint64_t)n)));
    neg = a3456_add(neg, a3456_mul(a3456_add(Q[n], A3456_g(n)), a3456_U(2 * (uint64_t)n)));
#undef A3456_g
    pos = a3456_sub(pos, neg);
    free(G); free(Q); free(C); free(S); free(S2); free(H);

    while ((D & 1) == 0) { D >>= 1; e++; }
    o = D;
    if (e == 0) { r0 = pos.w[0]; r1 = pos.w[1]; }
    else {
        r0 = (pos.w[0] >> e) | (pos.w[1] << (64 - e));
        r1 = (pos.w[1] >> e) | (pos.w[2] << (64 - e));
    }
    X = ((unsigned __int128)r1 << 64) | r0;
    inv = o;
    for (k = 0; k < 7; k++) inv *= (unsigned __int128)2 - (unsigned __int128)o * inv;
    X *= inv;
    return u128_to_seq(X);
}

seq_t a(idx_t n)
{
    if (a3456_fixed()) {
        if (n > 90) { seq_t z = 0; return z; } /* value cannot fit any fixed width */
        return a3456_fix(n);
    }
    return a3456_big(n);
}
