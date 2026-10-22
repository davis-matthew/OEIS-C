/*
 * A003778  Number of Hamiltonian paths in P_5 X P_n.
 * https://oeis.org/A003778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   transfer-matrix count of Hamiltonian paths in the 5 X n grid
 *            (column-by-column plug DP) for the first terms; Berlekamp-Massey
 *            (mod large primes, CRT-lifted to exact signed integers) recovers
 *            the linear recurrence a(n) = Sum_{i=1..L} c_i a(n-i), which is
 *            then used for all larger n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, gs, mulmod, release, ss, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/gs.h"
#include "../utils/mulmod.h"
#include "../utils/ss.h"

#define WD 5
#define NS 4096
#define NT 240

static int ntr[WD][NS];
static int tgt[WD][NS][3];

static int partner(int c, int i)
{
    int v = gs(c, i), d = 0, j;
    if (v == 1) {
        for (j = i + 1; j <= WD; j++) {
            int w = gs(c, j);
            if (w == 1) d++;
            else if (w == 2) { if (d == 0) return j; d--; }
        }
    } else if (v == 2) {
        for (j = i - 1; j >= 0; j--) {
            int w = gs(c, j);
            if (w == 2) d++;
            else if (w == 1) { if (d == 0) return j; d--; }
        }
    }
    return -1;
}

static void build(void)
{
    int r, c, i;
    for (r = 0; r < WD; r++)
        for (c = 0; c < NS; c++) {
            int k = 0, u = gs(c, r), l = gs(c, r + 1), n3 = 0, j;
            int base = ss(ss(c, r, 0), r + 1, 0);
            int cd = r < WD - 1;
            for (i = 0; i <= WD; i++) if (gs(c, i) == 3) n3++;
            if (u == 0 && l == 0) {
                if (cd) tgt[r][c][k++] = base | (1 << (2 * r)) | (2 << (2 * (r + 1)));
                if (n3 < 2) {
                    tgt[r][c][k++] = base | (3 << (2 * r));
                    if (cd) tgt[r][c][k++] = base | (3 << (2 * (r + 1)));
                }
            } else if (u == 0 || l == 0) {
                int x = u ? u : l, pos = u ? r : r + 1;
                tgt[r][c][k++] = base | (x << (2 * r));
                if (cd) tgt[r][c][k++] = base | (x << (2 * (r + 1)));
                if (x == 3) {
                    if (base == 0) tgt[r][c][k++] = -1;
                } else if (n3 < 2) {
                    j = partner(c, pos);
                    if (j >= 0) tgt[r][c][k++] = ss(base, j, 3);
                }
            } else {
                if (u == 1 && l == 2) { /* cycle */ }
                else if (u == 3 && l == 3) { if (base == 0) tgt[r][c][k++] = -1; }
                else if (u == 3) { j = partner(c, r + 1); if (j >= 0) tgt[r][c][k++] = ss(base, j, 3); }
                else if (l == 3) { j = partner(c, r); if (j >= 0) tgt[r][c][k++] = ss(base, j, 3); }
                else if (u == 1 && l == 1) { j = partner(c, r + 1); if (j >= 0) tgt[r][c][k++] = ss(base, j, 1); }
                else if (u == 2 && l == 2) { j = partner(c, r); if (j >= 0) tgt[r][c][k++] = ss(base, j, 2); }
                else tgt[r][c][k++] = base;
            }
            ntr[r][c] = k;
        }
}

static uint64_t powmod(uint64_t b, uint64_t e, uint64_t p)
{
    uint64_t r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = mulmod(r, b, p);
        b = mulmod(b, b, p);
        e >>= 1;
    }
    return r;
}

static bool mr_prime(uint64_t n)
{
    static const uint64_t bs[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t d;
    int s = 0, i, k;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bs[i]) return true;
        if (n % bs[i] == 0) return false;
    }
    d = n - 1;
    while (!(d & 1)) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = powmod(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (k = 1; k < s; k++) {
            x = mulmod(x, x, n);
            if (x == n - 1) break;
        }
        if (k == s) return false;
    }
    return true;
}

static uint64_t DA[NS], DB[NS];

static void dp_mod(uint64_t p, uint64_t *out, int N)
{
    int col, r, c, t;
    memset(DA, 0, sizeof DA);
    DA[0] = 1;
    for (col = 0; col < N; col++) {
        uint64_t done = 0;
        for (r = 0; r < WD; r++) {
            memset(DB, 0, sizeof DB);
            done = 0;
            for (c = 0; c < NS; c++) {
                if (!DA[c]) continue;
                for (t = 0; t < ntr[r][c]; t++) {
                    int v = tgt[r][c][t];
                    if (v < 0) { done += DA[c]; if (done >= p) done -= p; }
                    else { DB[v] += DA[c]; if (DB[v] >= p) DB[v] -= p; }
                }
            }
            memcpy(DA, DB, sizeof DA);
        }
        out[col] = done;
        memset(DB, 0, sizeof DB);
        for (c = 0; c < NS; c++) if (DA[c]) DB[(c << 2) & (NS - 1)] = DA[c];
        memcpy(DA, DB, sizeof DA);
    }
}

static void dp_seq(seq_t *out, int N)
{
    seq_t *A = alloc_zero(NS), *B = alloc_zero(NS), *T;
    int col, r, c, t;
    A[0] = 1;
    for (col = 0; col < N; col++) {
        seq_t done = 0;
        for (r = 0; r < WD; r++) {
            for (c = 0; c < NS; c++) B[c] = 0;
            done = 0;
            for (c = 0; c < NS; c++) {
                if (A[c] == 0) continue;
                for (t = 0; t < ntr[r][c]; t++) {
                    int v = tgt[r][c][t];
                    if (v < 0) done += A[c];
                    else B[v] += A[c];
                }
            }
            T = A; A = B; B = T;
        }
        out[col] = done;
        for (c = 0; c < NS; c++) B[c] = 0;
        for (c = 0; c < NS; c++) if (!(A[c] == 0)) B[(c << 2) & (NS - 1)] = A[c];
        T = A; A = B; B = T;
    }
    release(A, NS);
    release(B, NS);
}

static int bm(const uint64_t *s, int N, uint64_t p, uint64_t *C)
{
    static uint64_t B[NT + 1], T[NT + 1];
    int L = 0, m = 1, n, i, j;
    uint64_t b = 1;
    memset(B, 0, sizeof B);
    for (i = 0; i <= N; i++) C[i] = 0;
    C[0] = B[0] = 1;
    for (n = 0; n < N; n++) {
        uint64_t d = s[n] % p, coef;
        for (i = 1; i <= L; i++) {
            d += mulmod(C[i], s[n - i], p);
            if (d >= p) d -= p;
        }
        if (d == 0) { m++; continue; }
        coef = mulmod(d, powmod(b, p - 2, p), p);
        if (2 * L <= n) {
            memcpy(T, C, sizeof(uint64_t) * (N + 1));
            for (j = 0; j + m <= N; j++)
                C[j + m] = (C[j + m] + p - mulmod(coef, B[j], p)) % p;
            L = n + 1 - L;
            memcpy(B, T, sizeof(uint64_t) * (N + 1));
            b = d;
            m = 1;
        } else {
            for (j = 0; j + m <= N; j++)
                C[j + m] = (C[j + m] + p - mulmod(coef, B[j], p)) % p;
            m++;
        }
    }
    return L;
}

static uint64_t prevprime(uint64_t x)
{
    do x--; while (!mr_prime(x));
    return x;
}

static int ready = 0, L = 0;
static seq_t *cf = NULL;
static signed char sg[NT + 1];
static seq_t *vals = NULL;
static idx_t cap = 0, have = 0;

static void setup(void)
{
    static uint64_t s1[NT], s2[NT], s3[NT], C1[NT + 1], C2[NT + 1], C3[NT + 1];
    static unsigned __int128 mag[NT + 1];
    uint64_t ps[8];
    int i, t, M;
    build();
    ps[0] = prevprime(1ULL << 62);
    for (i = 1; i < 8; i++) ps[i] = prevprime(ps[i - 1]);
    for (t = 0; t + 2 < 8; t++) {
        uint64_t p1 = ps[t], p2 = ps[t + 1], p3 = ps[t + 2], inv;
        unsigned __int128 Mo;
        int L1, L2, L3, ok = 1;
        dp_mod(p1, s1, NT); dp_mod(p2, s2, NT); dp_mod(p3, s3, NT);
        L1 = bm(s1, NT, p1, C1); L2 = bm(s2, NT, p2, C2); L3 = bm(s3, NT, p3, C3);
        if (L1 != L2 || L1 != L3) continue;
        L = L1;
        inv = powmod(p1 % p2, p2 - 2, p2);
        Mo = (unsigned __int128)p1 * p2;
        for (i = 1; i <= L; i++) {
            uint64_t r1 = (p1 - C1[i]) % p1, r2 = (p2 - C2[i]) % p2, r3 = (p3 - C3[i]) % p3, h, xm;
            unsigned __int128 x;
            h = mulmod((r2 + p2 - r1 % p2) % p2, inv, p2);
            x = (unsigned __int128)r1 + (unsigned __int128)p1 * h;
            if (x > Mo / 2) { sg[i] = -1; mag[i] = Mo - x; }
            else { sg[i] = (x != 0) ? 1 : 0; mag[i] = x; }
            xm = (uint64_t)(mag[i] % p3);
            if (sg[i] < 0) xm = (p3 - xm) % p3;
            if (xm != r3) ok = 0;
        }
        if (ok) break;
    }
    cf = alloc_zero((idx_t)L + 1);
    for (i = 1; i <= L; i++) cf[i] = u128_to_seq(mag[i]);
    M = L > 0 ? L : 1;
    cap = (idx_t)(M > 64 ? M : 64);
    vals = alloc_zero(cap);
    dp_seq(vals, M);
    have = (idx_t)M;
    ready = 1;
}

seq_t a(idx_t n)
{
    idx_t k;
    int i;
    if (!ready) setup();
    if (n > have) {
        if (n > cap) {
            idx_t nc = cap * 2 > n ? cap * 2 : n;
            vals = grow_zero(vals, cap, nc);
            cap = nc;
        }
        for (k = have; k < n; k++) {
            seq_t pos = 0, neg = 0;
            for (i = 1; i <= L; i++) {
                if (sg[i] > 0) pos += cf[i] * vals[k - i];
                else if (sg[i] < 0) neg += cf[i] * vals[k - i];
            }
            vals[k] = pos - neg;
        }
        have = n;
    }
    return vals[n - 1];
}
