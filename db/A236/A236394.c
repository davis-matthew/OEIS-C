/*
 * A236394  Primes produced by A064526, in order of appearance.
 * https://oeis.org/A236394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search -- iterate A064526 in internal multiprecision (q(n+1) =
 *            p(n)*q(n-1), p(n+1) = q(n+1) + q(n), p(1) = q(0) = q(1) = 1),
 *            fully factor each p(n) by trial division + Pollard-Brent rho
 *            (Montgomery arithmetic) + Miller-Rabin, and append the primes
 *            not yet seen, in increasing order within each p(n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define B_ML 24
typedef unsigned __int128 B_u128;
typedef struct { uint64_t d[B_ML]; int n; } B_bn;

static void B_norm(B_bn *a) { while (a->n > 0 && a->d[a->n - 1] == 0) a->n--; }
static void B_set(B_bn *a, uint64_t v)
{
    int i;
    for (i = 0; i < B_ML; i++) a->d[i] = 0;
    a->d[0] = v; a->n = v ? 1 : 0;
}
static int B_cmp(const B_bn *a, const B_bn *b)
{
    int i;
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (i = a->n - 1; i >= 0; i--)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}
static bool B_isone(const B_bn *a) { return a->n == 1 && a->d[0] == 1; }
static void B_sub(B_bn *a, const B_bn *b)
{
    uint64_t br = 0; int i;
    for (i = 0; i < a->n; i++) {
        uint64_t bi = b->d[i], t = a->d[i] - bi, b1 = a->d[i] < bi;
        uint64_t t2 = t - br, b2 = t < br;
        a->d[i] = t2; br = b1 | b2;
    }
    B_norm(a);
}
static bool B_add(B_bn *r, const B_bn *a, const B_bn *b)
{
    int m = a->n > b->n ? a->n : b->n, i; uint64_t c = 0; B_bn t;
    B_set(&t, 0);
    for (i = 0; i < m; i++) {
        B_u128 s = (B_u128)a->d[i] + b->d[i] + c;
        t.d[i] = (uint64_t)s; c = (uint64_t)(s >> 64);
    }
    if (c) { if (m >= B_ML) return false; t.d[m] = c; m++; }
    t.n = m; B_norm(&t); *r = t; return true;
}
static bool B_mul(B_bn *r, const B_bn *a, const B_bn *b)
{
    B_bn t; int i, j;
    B_set(&t, 0);
    if (a->n == 0 || b->n == 0) { *r = t; return true; }
    if (a->n + b->n > B_ML) return false;
    for (i = 0; i < a->n; i++) {
        uint64_t c = 0;
        for (j = 0; j < b->n; j++) {
            B_u128 s = (B_u128)a->d[i] * b->d[j] + t.d[i + j] + c;
            t.d[i + j] = (uint64_t)s; c = (uint64_t)(s >> 64);
        }
        t.d[i + b->n] = c;
    }
    t.n = a->n + b->n; B_norm(&t); *r = t; return true;
}
static void B_divsmall(B_bn *a, uint64_t d)
{
    B_u128 r = 0; int i;
    for (i = a->n - 1; i >= 0; i--) {
        B_u128 cur = (r << 64) | a->d[i];
        a->d[i] = (uint64_t)(cur / d); r = cur % d;
    }
    B_norm(a);
}
static uint64_t B_modsmall(const B_bn *a, uint64_t d)
{
    B_u128 r = 0; int i;
    for (i = a->n - 1; i >= 0; i--) r = ((r << 64) | a->d[i]) % d;
    return (uint64_t)r;
}
static void B_shr1(B_bn *a)
{
    int i;
    for (i = 0; i < a->n; i++)
        a->d[i] = (a->d[i] >> 1) | (i + 1 < a->n ? a->d[i + 1] << 63 : 0);
    B_norm(a);
}
static void B_shl1(B_bn *a)
{
    int i; uint64_t c = 0;
    for (i = 0; i < a->n; i++) {
        uint64_t nc = a->d[i] >> 63;
        a->d[i] = (a->d[i] << 1) | c; c = nc;
    }
    if (c && a->n < B_ML) { a->d[a->n] = c; a->n++; }
}
static int B_bits(const B_bn *a)
{
    int b; uint64_t t;
    if (!a->n) return 0;
    b = 64 * (a->n - 1); t = a->d[a->n - 1];
    while (t) { b++; t >>= 1; }
    return b;
}
static void B_div(B_bn *q, const B_bn *a, const B_bn *b)
{
    B_bn r; int i, nb = B_bits(a);
    B_set(q, 0); B_set(&r, 0);
    for (i = nb - 1; i >= 0; i--) {
        B_shl1(&r);
        if ((a->d[i >> 6] >> (i & 63)) & 1) { if (r.n == 0) r.n = 1; r.d[0] |= 1; }
        if (B_cmp(&r, b) >= 0) { B_sub(&r, b); q->d[i >> 6] |= (uint64_t)1 << (i & 63); }
    }
    q->n = B_ML; B_norm(q);
}
static void B_gcd(B_bn *g, const B_bn *a0, const B_bn *b0)
{
    B_bn a = *a0, b = *b0; int k = 0;
    if (a.n == 0) { *g = b; return; }
    if (b.n == 0) { *g = a; return; }
    while (!(a.d[0] & 1) && !(b.d[0] & 1)) { B_shr1(&a); B_shr1(&b); k++; }
    while (!(a.d[0] & 1)) B_shr1(&a);
    while (b.n) {
        while (!(b.d[0] & 1)) B_shr1(&b);
        if (B_cmp(&a, &b) > 0) { B_bn t = a; a = b; b = t; }
        B_sub(&b, &a);
    }
    while (k--) B_shl1(&a);
    *g = a;
}

/* ---- fixed-length L-limb modular arithmetic ---- */
static int L_cmp(const uint64_t *a, const uint64_t *b, int L)
{
    int i;
    for (i = L - 1; i >= 0; i--) if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}
static uint64_t L_sub(uint64_t *r, const uint64_t *a, const uint64_t *b, int L)
{
    uint64_t br = 0; int i;
    for (i = 0; i < L; i++) {
        uint64_t bi = b[i], t = a[i] - bi, b1 = a[i] < bi;
        uint64_t t2 = t - br, b2 = t < br;
        r[i] = t2; br = b1 | b2;
    }
    return br;
}
static void L_addmod(uint64_t *r, const uint64_t *a, const uint64_t *b, const uint64_t *N, int L)
{
    uint64_t c = 0; int i;
    for (i = 0; i < L; i++) {
        B_u128 s = (B_u128)a[i] + b[i] + c;
        r[i] = (uint64_t)s; c = (uint64_t)(s >> 64);
    }
    if (c || L_cmp(r, N, L) >= 0) L_sub(r, r, N, L);
}
static void L_absdiff(uint64_t *r, const uint64_t *a, const uint64_t *b, int L)
{
    if (L_cmp(a, b, L) >= 0) L_sub(r, a, b, L); else L_sub(r, b, a, L);
}
static void M_mul(uint64_t *r, const uint64_t *a, const uint64_t *b,
                  const uint64_t *N, int L, uint64_t ninv)
{
    uint64_t t[B_ML + 2]; int i, j; bool ge;
    for (i = 0; i < L + 2; i++) t[i] = 0;
    for (i = 0; i < L; i++) {
        uint64_t c = 0, m; B_u128 s;
        for (j = 0; j < L; j++) {
            s = (B_u128)a[j] * b[i] + t[j] + c;
            t[j] = (uint64_t)s; c = (uint64_t)(s >> 64);
        }
        s = (B_u128)t[L] + c; t[L] = (uint64_t)s; t[L + 1] = (uint64_t)(s >> 64);
        m = t[0] * ninv;
        s = (B_u128)m * N[0] + t[0]; c = (uint64_t)(s >> 64);
        for (j = 1; j < L; j++) {
            s = (B_u128)m * N[j] + t[j] + c;
            t[j - 1] = (uint64_t)s; c = (uint64_t)(s >> 64);
        }
        s = (B_u128)t[L] + c; t[L - 1] = (uint64_t)s; t[L] = t[L + 1] + (uint64_t)(s >> 64);
    }
    ge = t[L] != 0;
    if (!ge) ge = L_cmp(t, N, L) >= 0;
    if (ge) L_sub(t, t, N, L);
    for (i = 0; i < L; i++) r[i] = t[i];
}
static uint64_t M_ninv(uint64_t n0)
{
    uint64_t x = n0; int i;
    for (i = 0; i < 6; i++) x *= 2 - n0 * x;
    return (uint64_t)0 - x;
}
static void L_tobn(B_bn *r, const uint64_t *a, int L)
{
    int i;
    B_set(r, 0);
    for (i = 0; i < L; i++) r->d[i] = a[i];
    r->n = L; B_norm(r);
}

static bool B_isprime(const B_bn *Nb)
{
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t one[B_ML], R2[B_ML], m1[B_ML], am[B_ML], x[B_ML], ab[B_ML], zero[B_ML];
    const uint64_t *N = Nb->d; int L = Nb->n, i, bi, s = 0, nb;
    uint64_t ninv; B_bn d;
    if (L == 0 || (L == 1 && N[0] < 2)) return false;
    for (i = 0; i < 12; i++) {
        if (L == 1 && N[0] == bases[i]) return true;
        if (B_modsmall(Nb, bases[i]) == 0) return false;
    }
    if (L == 1 && N[0] < 1369) return true;
    ninv = M_ninv(N[0]);
    for (i = 0; i < L; i++) { one[i] = 0; zero[i] = 0; }
    one[0] = 1;
    for (i = 0; i < 64 * L; i++) L_addmod(one, one, one, N, L);
    for (i = 0; i < L; i++) R2[i] = one[i];
    for (i = 0; i < 64 * L; i++) L_addmod(R2, R2, R2, N, L);
    L_sub(m1, N, one, L);
    (void)zero;
    d = *Nb; d.d[0] -= 1; B_norm(&d);
    while (!(d.d[0] & 1)) { B_shr1(&d); s++; }
    nb = B_bits(&d);
    for (bi = 0; bi < 12; bi++) {
        int r; bool ok = false;
        for (i = 0; i < L; i++) ab[i] = 0;
        ab[0] = bases[bi];
        M_mul(am, ab, R2, N, L, ninv);
        for (i = 0; i < L; i++) x[i] = one[i];
        for (i = nb - 1; i >= 0; i--) {
            M_mul(x, x, x, N, L, ninv);
            if ((d.d[i >> 6] >> (i & 63)) & 1) M_mul(x, x, am, N, L, ninv);
        }
        if (L_cmp(x, one, L) == 0 || L_cmp(x, m1, L) == 0) continue;
        for (r = 1; r < s; r++) {
            M_mul(x, x, x, N, L, ninv);
            if (L_cmp(x, m1, L) == 0) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static bool B_rho(const B_bn *Nb, B_bn *g)
{
    const uint64_t *N = Nb->d; int L = Nb->n, i;
    uint64_t ninv = M_ninv(N[0]), c;
    for (c = 1; c < 64; c++) {
        uint64_t y[B_ML], x[B_ML], ys[B_ML], q[B_ML], cc[B_ML], diff[B_ML];
        unsigned long r = 1, k, it, m = 128, lim;
        B_bn G;
        for (i = 0; i < L; i++) { y[i] = 0; q[i] = 0; cc[i] = 0; }
        y[0] = 2; q[0] = 1; cc[0] = c;
        B_set(&G, 1);
        do {
            for (i = 0; i < L; i++) x[i] = y[i];
            for (it = 0; it < r; it++) { M_mul(y, y, y, N, L, ninv); L_addmod(y, y, cc, N, L); }
            k = 0;
            while (k < r && B_isone(&G)) {
                B_bn qb;
                for (i = 0; i < L; i++) ys[i] = y[i];
                lim = (r - k < m) ? r - k : m;
                for (it = 0; it < lim; it++) {
                    M_mul(y, y, y, N, L, ninv); L_addmod(y, y, cc, N, L);
                    L_absdiff(diff, x, y, L);
                    M_mul(q, q, diff, N, L, ninv);
                }
                L_tobn(&qb, q, L);
                B_gcd(&G, &qb, Nb);
                k += m;
            }
            r *= 2;
            if (r > (1UL << 27)) break;
        } while (B_isone(&G));
        if (B_isone(&G)) continue;
        if (B_cmp(&G, Nb) == 0) {
            do {
                B_bn db;
                M_mul(ys, ys, ys, N, L, ninv); L_addmod(ys, ys, cc, N, L);
                L_absdiff(diff, x, ys, L);
                L_tobn(&db, diff, L);
                B_gcd(&G, &db, Nb);
            } while (B_isone(&G));
        }
        if (B_cmp(&G, Nb) != 0 && !B_isone(&G)) { *g = G; return true; }
    }
    return false;
}

#define F_MAX (B_ML * 64 + 8)
static B_bn F_buf[F_MAX];
static int F_cnt = 0;

static bool F_push(const B_bn *p)
{
    if (F_cnt >= F_MAX) return false;
    F_buf[F_cnt++] = *p;
    return true;
}
static bool F_big(const B_bn *n)
{
    B_bn g, q;
    if (n->n == 0 || B_isone(n)) return true;
    if (B_isprime(n)) return F_push(n);
    if (!B_rho(n, &g)) return false;
    B_div(&q, n, &g);
    return F_big(&g) && F_big(&q);
}
static bool F_factor(B_bn n)
{
    uint64_t d;
    F_cnt = 0;
    if (n.n == 0) return true;
    for (d = 2; d < 10000; d += (d == 2 ? 1 : 2)) {
        if (n.n == 1 && n.d[0] < d * d) break;
        if (B_modsmall(&n, d) == 0) {
            B_bn p; B_set(&p, d);
            if (!F_push(&p)) return false;
            while (B_modsmall(&n, d) == 0) B_divsmall(&n, d);
        }
    }
    return F_big(&n);
}

#define P_MAX 4096
static B_bn P_list[P_MAX];
static size_t P_cnt = 0;
static bool S_init = false, S_dead = false;
static B_bn S_cp, S_pq, S_cq;

static bool S_step(void)
{
    B_bn qi, ai; int i, j; size_t t;
    if (!B_mul(&qi, &S_cp, &S_pq)) return false;
    if (!B_add(&ai, &qi, &S_cq)) return false;
    S_pq = S_cq; S_cq = qi; S_cp = ai;
    if (!F_factor(ai)) return false;
    for (i = 1; i < F_cnt; i++) {
        B_bn v = F_buf[i];
        j = i;
        while (j > 0 && B_cmp(&F_buf[j - 1], &v) > 0) { F_buf[j] = F_buf[j - 1]; j--; }
        F_buf[j] = v;
    }
    for (i = 0; i < F_cnt; i++) {
        bool seen = false;
        for (t = 0; t < P_cnt; t++) if (B_cmp(&P_list[t], &F_buf[i]) == 0) { seen = true; break; }
        if (!seen) {
            if (P_cnt >= P_MAX) return false;
            P_list[P_cnt++] = F_buf[i];
        }
    }
    return true;
}

static seq_t B_toseq(const B_bn *a)
{
    seq_t v = 0; int i, k;
    for (i = a->n - 1; i >= 0; i--)
        for (k = 3; k >= 0; k--)
            v = v * (seq_t)65536u + (seq_t)((a->d[i] >> (16 * k)) & 0xFFFFu);
    return v;
}

seq_t a(idx_t n)
{
    if (!S_init) {
        S_init = true;
        B_set(&S_cp, 1); B_set(&S_pq, 1); B_set(&S_cq, 1);
    }
    while (P_cnt < (size_t)n && !S_dead) {
        if (!S_step()) S_dead = true;
    }
    if (n >= 1 && (size_t)n <= P_cnt) return B_toseq(&P_list[n - 1]);
    return (seq_t)0;
}
