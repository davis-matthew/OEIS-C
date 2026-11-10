/*
 * A068658  Primes in A068657.
 * https://oeis.org/A068658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th prime among A068657(k), k = 1, 2, 3, ... (search
 *            over k: build A068657(k), trial-divide, then strong
 *            probable-prime tests on the survivors)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 13, u64 n <= 52, u128 n <= 575,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 9945 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

static seq_t A068657_a(idx_t n)
{
    seq_t result = (seq_t)0;
    idx_t cur = n;
    while (true) {
        seq_t part = (seq_t)cur;
        seq_t pow10 = (seq_t)1;
        seq_t temp = part;
        unsigned int digits = 0;
        do {
            ++digits;
            temp /= (seq_t)10;
        } while (temp != (seq_t)0);
        for (unsigned int i = 0; i < digits; ++i)
            pow10 *= (seq_t)10;
        result = result * pow10 + part;
        if (cur == (idx_t)1)
            break;
        cur /= (idx_t)2;
    }
    return result;
}

#define A068658_MAXL 72

typedef unsigned __int128 A068658_u128;

static uint32_t A068658_gmod[64];
static int A068658_gcnt = 0;
static uint32_t A068658_pr[128];
static int A068658_gstart[65];
static bool A068658_inited = false;

static void A068658_init(void)
{
    const int LIM = 256;
    bool comp[257];
    int i, j, np = 0;
    uint64_t cur = 1;
    memset(comp, 0, sizeof(comp));
    for (i = 2; i <= LIM; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= LIM; j += i) comp[j] = true;
    }
    A068658_gcnt = 0;
    A068658_gstart[0] = 0;
    for (i = 3; i <= LIM; i++) {
        if (comp[i] || i == 5) continue;
        if (cur * (uint64_t)i >= ((uint64_t)1 << 32)) {
            A068658_gmod[A068658_gcnt] = (uint32_t)cur;
            A068658_gcnt++;
            A068658_gstart[A068658_gcnt] = np;
            cur = 1;
        }
        A068658_pr[np++] = (uint32_t)i;
        cur *= (uint64_t)i;
    }
    if (cur > 1) {
        A068658_gmod[A068658_gcnt] = (uint32_t)cur;
        A068658_gcnt++;
        A068658_gstart[A068658_gcnt] = np;
    }
    A068658_inited = true;
}

static void A068658_mul_small(uint64_t *L, int *s, uint64_t m)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < *s; i++) {
        A068658_u128 x = (A068658_u128)L[i] * m + c;
        L[i] = (uint64_t)x;
        c = (uint64_t)(x >> 64);
    }
    if (c) L[(*s)++] = c;
}

static void A068658_add_u64(uint64_t *L, int *s, uint64_t v)
{
    uint64_t c = v;
    int i = 0;
    while (c) {
        if (i >= *s) { L[i] = 0; (*s)++; }
        uint64_t x = L[i] + c;
        c = (x < c) ? 1 : 0;
        L[i] = x;
        i++;
    }
}

/* builds A068657(k) as little-endian 64-bit limbs; returns limb count */
static int A068658_build(idx_t k, uint64_t *L)
{
    idx_t parts[70];
    int np = 0, s = 0, i;
    idx_t cur = k;
    while (true) {
        parts[np++] = cur;
        if (cur == 1) break;
        cur /= 2;
    }
    for (i = 0; i < np; i++) {
        idx_t p = parts[i];
        int d = 0;
        idx_t t = p;
        do { d++; t /= 10; } while (t);
        while (d > 0) {
            int step = d > 18 ? 18 : d;
            uint64_t m = 1;
            int e;
            for (e = 0; e < step; e++) m *= 10;
            A068658_mul_small(L, &s, m);
            d -= step;
        }
        A068658_add_u64(L, &s, (uint64_t)p);
    }
    while (s > 1 && L[s - 1] == 0) s--;
    if (s == 0) { L[0] = 0; s = 1; }
    return s;
}

static int A068658_cmp(const uint64_t *a, const uint64_t *b, int s)
{
    int i;
    for (i = s - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void A068658_sub(uint64_t *a, const uint64_t *b, int s)
{
    uint64_t br = 0;
    int i;
    for (i = 0; i < s; i++) {
        uint64_t ai = a[i], bi = b[i];
        uint64_t d1 = ai - bi;
        uint64_t b1 = ai < bi;
        uint64_t d2 = d1 - br;
        uint64_t b2 = d1 < br;
        a[i] = d2;
        br = b1 | b2;
    }
}

static void A068658_mont(uint64_t *r, const uint64_t *a, const uint64_t *b,
                         const uint64_t *N, int s, uint64_t ninv)
{
    uint64_t t[A068658_MAXL + 2];
    int i, j;
    memset(t, 0, sizeof(uint64_t) * (size_t)(s + 2));
    for (i = 0; i < s; i++) {
        A068658_u128 x;
        uint64_t C = 0, bi = b[i], m;
        for (j = 0; j < s; j++) {
            x = (A068658_u128)t[j] + (A068658_u128)a[j] * bi + C;
            t[j] = (uint64_t)x;
            C = (uint64_t)(x >> 64);
        }
        x = (A068658_u128)t[s] + C;
        t[s] = (uint64_t)x;
        t[s + 1] = (uint64_t)(x >> 64);
        m = t[0] * ninv;
        x = (A068658_u128)t[0] + (A068658_u128)m * N[0];
        C = (uint64_t)(x >> 64);
        for (j = 1; j < s; j++) {
            x = (A068658_u128)t[j] + (A068658_u128)m * N[j] + C;
            t[j - 1] = (uint64_t)x;
            C = (uint64_t)(x >> 64);
        }
        x = (A068658_u128)t[s] + C;
        t[s - 1] = (uint64_t)x;
        C = (uint64_t)(x >> 64);
        t[s] = t[s + 1] + C;
    }
    if (t[s] != 0 || A068658_cmp(t, N, s) >= 0)
        A068658_sub(t, N, s);
    memcpy(r, t, sizeof(uint64_t) * (size_t)s);
}

static bool A068658_bit(const uint64_t *N, int b)
{
    return ((N[b >> 6] >> (b & 63)) & 1u) != 0;
}

/* strong probable-prime tests on odd N (s >= 2 limbs) */
static bool A068658_mr(const uint64_t *N, int s)
{
    static const uint64_t bases[5] = {2, 3, 5, 7, 11};
    uint64_t R2[A068658_MAXL], one[A068658_MAXL], onem[A068658_MAXL];
    uint64_t mone[A068658_MAXL], aM[A068658_MAXL], x[A068658_MAXL], tmp[A068658_MAXL];
    uint64_t inv = N[0], ninv;
    int i, b, top, sh, bi;

    for (i = 0; i < 6; i++) inv *= (uint64_t)2 - N[0] * inv;
    ninv = (uint64_t)0 - inv;

    memset(R2, 0, sizeof(uint64_t) * (size_t)s);
    R2[0] = 1;
    for (i = 0; i < 128 * s; i++) {
        uint64_t carry = 0;
        int j;
        for (j = 0; j < s; j++) {
            uint64_t nc = R2[j] >> 63;
            R2[j] = (R2[j] << 1) | carry;
            carry = nc;
        }
        if (carry || A068658_cmp(R2, N, s) >= 0)
            A068658_sub(R2, N, s);
    }
    memset(one, 0, sizeof(uint64_t) * (size_t)s);
    one[0] = 1;
    A068658_mont(onem, one, R2, N, s, ninv);
    memcpy(mone, N, sizeof(uint64_t) * (size_t)s);
    A068658_sub(mone, onem, s);

    top = 64 * s - 1;
    while (!A068658_bit(N, top)) top--;
    sh = 1;
    while (!A068658_bit(N, sh)) sh++;

    for (bi = 0; bi < 5; bi++) {
        bool ok;
        memset(tmp, 0, sizeof(uint64_t) * (size_t)s);
        tmp[0] = bases[bi];
        A068658_mont(aM, tmp, R2, N, s, ninv);
        memcpy(x, aM, sizeof(uint64_t) * (size_t)s);
        for (b = top - 1; b >= sh; b--) {
            A068658_mont(x, x, x, N, s, ninv);
            if (A068658_bit(N, b))
                A068658_mont(x, x, aM, N, s, ninv);
        }
        if (A068658_cmp(x, onem, s) == 0 || A068658_cmp(x, mone, s) == 0)
            continue;
        ok = false;
        for (i = 1; i < sh; i++) {
            A068658_mont(x, x, x, N, s, ninv);
            if (A068658_cmp(x, mone, s) == 0) { ok = true; break; }
            if (A068658_cmp(x, onem, s) == 0) break;
        }
        if (!ok) return false;
    }
    return true;
}

static bool A068658_test(idx_t k)
{
    uint64_t L[A068658_MAXL];
    int s = A068658_build(k, L);
    int g, i;
    if (s <= 1)
        return is_prime_u64(L[0]);
    if ((L[0] & 1u) == 0) return false;
    for (g = 0; g < A068658_gcnt; g++) {
        uint64_t M = A068658_gmod[g], r = 0;
        for (i = s - 1; i >= 0; i--) {
            r = ((r << 32) | (L[i] >> 32)) % M;
            r = ((r << 32) | (L[i] & 0xFFFFFFFFu)) % M;
        }
        for (i = A068658_gstart[g]; i < A068658_gstart[g + 1]; i++)
            if (r % A068658_pr[i] == 0) return false;
    }
    return A068658_mr(L, s);
}

static idx_t *A068658_ks = 0;
static size_t A068658_cnt = 0;
static size_t A068658_cap = 0;
static idx_t A068658_next = 1;

seq_t a(idx_t n)
{
    if (!A068658_inited) A068658_init();
    while (A068658_cnt < (size_t)n) {
        idx_t k = A068658_next++;
        if (A068658_test(k)) {
            if (A068658_cnt == A068658_cap) {
                size_t nc = A068658_cap ? A068658_cap * 2 : 64;
                A068658_ks = (idx_t *)realloc(A068658_ks, nc * sizeof(idx_t));
                A068658_cap = nc;
            }
            A068658_ks[A068658_cnt++] = k;
        }
    }
    return A068657_a(A068658_ks[n - 1]);
}
