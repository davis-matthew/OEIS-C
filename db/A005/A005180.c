/*
 * A005180  Orders of simple groups.
 * https://oeis.org/A005180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   sieve/enumeration: a(n) is the n-th smallest element of {1} U
 *            primes U orders of all non-abelian finite simple groups
 *            (classification: alternating n!/2, Lie-type families PSL, PSU,
 *            B/C, D, 2D, G2, 3D4, F4, E6, 2E6, E7, E8, Suzuki, Ree, 2F4
 *            including the Tits group, and the sporadic groups), with...
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static uint64_t *L = NULL;
static size_t Lc = 0, Lcap = 0;
static uint64_t BND = 0;
static u128 CAP;

static void push(uint64_t v) {
    if (Lc >= Lcap) {
        Lcap = Lcap ? Lcap * 2 : 4096;
        L = (uint64_t *)realloc(L, Lcap * sizeof(uint64_t));
    }
    L[Lc++] = v;
}

static u128 cm(u128 a, u128 b) {
    if (a == 0 || b == 0) return 0;
    if (a > CAP / b) return CAP + 1;
    return a * b;
}

static u128 cpow(u128 q, unsigned e) {
    u128 r = 1;
    while (e--) r = cm(r, q);
    return r;
}

static uint64_t gcd64(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* returns false if the undivided order exceeded CAP */
static bool addo(u128 ord, u128 d) {
    if (ord > CAP) return false;
    u128 v = ord / d;
    if (v <= (u128)BND) push((uint64_t)v);
    return true;
}

static u128 exc(u128 q, unsigned N, const int *ex, const int *sg, int k) {
    u128 r = cpow(q, N);
    int i;
    for (i = 0; i < k; i++) {
        u128 t = cpow(q, (unsigned)ex[i]);
        r = cm(r, sg[i] > 0 ? t + 1 : t - 1);
    }
    return r;
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t a1 = *(const uint64_t *)x, b1 = *(const uint64_t *)y;
    return a1 < b1 ? -1 : (a1 > b1 ? 1 : 0);
}

static void family(uint64_t q, uint64_t p, unsigned k) {
    unsigned m;
    int i;
    /* PSL(m+1,q) */
    for (m = 1;; m++) {
        if (m == 1 && q <= 3) continue;
        u128 r = cpow(q, m * (m + 1) / 2);
        for (i = 2; i <= (int)m + 1; i++) r = cm(r, cpow(q, (unsigned)i) - 1);
        if (!addo(r, gcd64(m + 1, q - 1))) break;
    }
    /* PSU(m+1,q) */
    for (m = 2;; m++) {
        u128 r = cpow(q, m * (m + 1) / 2);
        for (i = 2; i <= (int)m + 1; i++) {
            u128 t = cpow(q, (unsigned)i);
            r = cm(r, (i & 1) ? t + 1 : t - 1);
        }
        if (m == 2 && q == 2) { if (r > CAP) break; continue; }
        if (!addo(r, gcd64(m + 1, q + 1))) break;
    }
    /* B_m(q) (same orders as C_m(q)) */
    for (m = 2;; m++) {
        u128 r = cpow(q, m * m);
        for (i = 1; i <= (int)m; i++) r = cm(r, cpow(q, 2 * (unsigned)i) - 1);
        if (m == 2 && q == 2) { if (r > CAP) break; continue; }
        if (!addo(r, (q & 1) ? 2 : 1)) break;
    }
    /* D_m(q), 2D_m(q) */
    for (m = 4;; m++) {
        u128 base = cpow(q, m * (m - 1));
        for (i = 1; i <= (int)m - 1; i++) base = cm(base, cpow(q, 2 * (unsigned)i) - 1);
        u128 qm = cpow(q, m);
        unsigned qm4 = 1, j;
        for (j = 0; j < m; j++) qm4 = (unsigned)((qm4 * (q % 4)) % 4);
        uint64_t d1, d2;
        if (q % 2 == 0) { d1 = 1; d2 = 1; }
        else { d1 = (qm4 == 1) ? 4 : 2; d2 = (qm4 == 3) ? 4 : 2; }
        bool ok1 = addo(cm(base, qm - 1), d1);
        bool ok2 = addo(cm(base, qm + 1), d2);
        if (!ok1 && !ok2) break;
    }
    /* exceptional groups */
    {
        static const int g2e[2] = {6, 2}, g2s[2] = {-1, -1};
        if (q >= 3) addo(exc(q, 6, g2e, g2s, 2), 1);
        static const int d4e[3] = {6, 2, 0}, d4s[2] = {-1, -1};
        u128 r = exc(q, 12, d4e, d4s, 2);
        u128 q4 = cpow(q, 4);
        r = cm(r, cm(q4, q4) + q4 + 1);
        addo(r, 1);
        static const int f4e[4] = {12, 8, 6, 2}, f4s[4] = {-1, -1, -1, -1};
        addo(exc(q, 24, f4e, f4s, 4), 1);
        static const int e6e[6] = {12, 9, 8, 6, 5, 2};
        static const int e6s[6] = {-1, -1, -1, -1, -1, -1};
        static const int e6t[6] = {-1, 1, -1, -1, 1, -1};
        addo(exc(q, 36, e6e, e6s, 6), gcd64(3, q - 1));
        addo(exc(q, 36, e6e, e6t, 6), gcd64(3, q + 1));
        static const int e7e[7] = {2, 6, 8, 10, 12, 14, 18};
        static const int e7s[7] = {-1, -1, -1, -1, -1, -1, -1};
        addo(exc(q, 63, e7e, e7s, 7), (q & 1) ? 2 : 1);
        static const int e8e[8] = {2, 8, 12, 14, 18, 20, 24, 30};
        static const int e8s[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
        addo(exc(q, 120, e8e, e8s, 8), 1);
    }
    /* Suzuki and 2F4: q = 2^(2k+1) >= 8 */
    if (p == 2 && (k & 1) && k >= 3) {
        static const int sze[2] = {2, 1}, szs[2] = {1, -1};
        addo(exc(q, 2, sze, szs, 2), 1);
        static const int f2e[4] = {6, 4, 3, 1}, f2s[4] = {1, -1, 1, -1};
        addo(exc(q, 12, f2e, f2s, 4), 1);
    }
    /* Ree 2G2: q = 3^(2k+1) >= 27 */
    if (p == 3 && (k & 1) && k >= 3) {
        static const int ree[2] = {3, 1}, res[2] = {1, -1};
        addo(exc(q, 3, ree, res, 2), 1);
    }
}

static void build(void) {
    uint64_t i, j;
    Lc = 0;
    CAP = (u128)BND * 64;
    push(1);
    unsigned char *comp = (unsigned char *)calloc((size_t)BND + 1, 1);
    for (i = 2; i <= BND; i++) {
        if (comp[i]) continue;
        push(i);
        if (i <= BND / i)
            for (j = i * i; j <= BND; j += i) comp[j] = 1;
    }
    /* alternating groups */
    {
        u128 f = 60;
        uint64_t m;
        for (m = 5; f <= (u128)BND; m++) {
            push((uint64_t)f);
            f = cm(f, m + 1);
        }
    }
    /* prime powers q with PSL(2,q)-sized order <= bound */
    uint64_t qmax = 2;
    while ((u128)qmax * ((u128)qmax * qmax - 1) / 2 <= (u128)BND) qmax++;
    for (i = 2; i <= qmax && i <= BND; i++) {
        if (comp[i]) continue;
        uint64_t q = i;
        unsigned k = 1;
        while (q <= qmax) {
            family(q, i, k);
            if (q > qmax / i) break;
            q *= i;
            k++;
        }
    }
    free(comp);
    /* Tits group and sporadic groups (those below 2^63) */
    {
        static const uint64_t spor[] = {
            17971200ULL, 7920ULL, 95040ULL, 443520ULL, 10200960ULL, 244823040ULL,
            175560ULL, 604800ULL, 50232960ULL, 4157776806543360000ULL,
            42305421312000ULL, 495766656000ULL, 64561751654400ULL,
            4089470473293004800ULL, 44352000ULL, 898128000ULL, 4030387200ULL,
            145926144000ULL, 448345497600ULL, 460815505920ULL,
            273030912000000000ULL, 51765179004000000ULL, 90745943887872000ULL
        };
        size_t s;
        for (s = 0; s < sizeof(spor) / sizeof(spor[0]); s++)
            if (spor[s] <= BND) push(spor[s]);
    }
    qsort(L, Lc, sizeof(uint64_t), cmp_u64);
    size_t w = 0, r;
    for (r = 0; r < Lc; r++)
        if (w == 0 || L[w - 1] != L[r]) L[w++] = L[r];
    Lc = w;
}

seq_t a(idx_t n) {
    while (Lc < n) {
        if (BND >= ((uint64_t)1 << 40)) return (seq_t)0;
        BND = BND ? BND * 2 : 1024;
        build();
    }
    return u128_to_seq((u128)L[n - 1]);
}
