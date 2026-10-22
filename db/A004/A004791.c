/*
 * A004791  Numbers k >= 2 such that if 1 <= j < k then fractional part of log
 *          k > fractional part of log j.
 * https://oeis.org/A004791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over blocks e^(m-1) < k < e^m (natural log). Within block
 *            m the fractional part of log k increases with k, so the terms in
 *            block m are the integers k with R*e^(m-L) < k <= floor(e^m),
 *            where R = floor(e^L) is the last record (from block L). e^m is
 *            computed exactly enough with multi-limb fix...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int128 a004791_u128;

static long g_cap = 0, g_s = 0, g_sh = 0, g_m = 0;
static uint64_t *g_mant = NULL, *g_E = NULL, *g_prod = NULL;
static uint64_t g_D[5], g_e5[5];
static uint64_t **g_F = NULL;
static long *g_Flen = NULL;
static uint64_t *g_cnt = NULL, *g_cum = NULL;
static long g_nb = 0, g_bcap = 0;
static uint64_t g_total = 0;

static uint64_t limb_at(const uint64_t *a, long len, long i)
{
    return (i < 0 || i >= len) ? 0 : a[i];
}

static uint64_t bits_at(const uint64_t *a, long len, long pos)
{
    long w = pos >= 0 ? pos / 64 : -((-pos + 63) / 64);
    int b = (int)(pos - 64 * w);
    uint64_t r = limb_at(a, len, w) >> b;
    if (b) r |= limb_at(a, len, w + 1) << (64 - b);
    return r;
}

static void init_state(long cap)
{
    long i, s, L, k;
    uint64_t *T, *S;
    for (i = 0; i < g_nb; i++) free(g_F[i]);
    free(g_F); free(g_Flen); free(g_cnt); free(g_cum);
    free(g_mant); free(g_E); free(g_prod);
    g_F = NULL; g_Flen = NULL; g_cnt = NULL; g_cum = NULL;
    g_nb = 0; g_bcap = 0; g_total = 0;

    g_cap = cap;
    s = (cap * 1443 / 1000 + 400) / 64 + 2;
    g_s = s;
    L = s + 2;
    T = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
    S = (uint64_t *)calloc((size_t)L, sizeof(uint64_t));
    T[L - 1] = 1;
    S[L - 1] = 1;
    for (k = 1;; k++) {
        a004791_u128 rem = 0;
        int nz = 0;
        uint64_t carry = 0;
        for (i = L - 1; i >= 0; i--) {
            a004791_u128 cur = (rem << 64) | T[i];
            T[i] = (uint64_t)(cur / (uint64_t)k);
            rem = cur % (uint64_t)k;
            if (T[i]) nz = 1;
        }
        if (!nz) break;
        for (i = 0; i < L; i++) {
            a004791_u128 t = (a004791_u128)S[i] + T[i] + carry;
            S[i] = (uint64_t)t;
            carry = (uint64_t)(t >> 64);
        }
    }
    g_E = (uint64_t *)malloc((size_t)s * sizeof(uint64_t));
    for (i = 0; i < s; i++) g_E[i] = bits_at(S, L, 66 + 64 * i);
    g_e5[4] = S[L - 1];
    g_e5[3] = S[L - 2];
    g_e5[2] = S[L - 3];
    g_e5[1] = S[L - 4];
    g_e5[0] = S[L - 5];
    free(T); free(S);

    g_mant = (uint64_t *)calloc((size_t)s, sizeof(uint64_t));
    g_prod = (uint64_t *)calloc((size_t)(2 * s), sizeof(uint64_t));
    g_mant[s - 1] = (uint64_t)1 << 62;
    g_sh = 64 * s - 2;
    g_m = 0;
    memset(g_D, 0, sizeof(g_D));
}

static void step(void)
{
    long s = g_s, i, j, k;
    uint64_t count;
    uint64_t p[10];

    memset(g_prod, 0, (size_t)(2 * s) * sizeof(uint64_t));
    for (i = 0; i < s; i++) {
        uint64_t carry = 0;
        long js = s - 2 - i;
        if (js < 0) js = 0;
        for (j = js; j < s; j++) {
            a004791_u128 t = (a004791_u128)g_mant[i] * g_E[j] + g_prod[i + j] + carry;
            g_prod[i + j] = (uint64_t)t;
            carry = (uint64_t)(t >> 64);
        }
        g_prod[i + s] = carry;
    }
    k = g_prod[2 * s - 1] ? s : s - 1;
    memcpy(g_mant, g_prod + k, (size_t)s * sizeof(uint64_t));
    g_sh += 64 * s - 2 - 64 * k;
    g_m++;

    memset(p, 0, sizeof(p));
    for (i = 0; i < 5; i++) {
        uint64_t carry = 0;
        for (j = 0; j < 5; j++) {
            a004791_u128 t = (a004791_u128)g_D[i] * g_e5[j] + p[i + j] + carry;
            p[i + j] = (uint64_t)t;
            carry = (uint64_t)(t >> 64);
        }
        p[i + 5] = carry;
    }
    for (i = 0; i < 5; i++) g_D[i] = p[i + 4];

    if (g_m == 1) {
        count = 1;
    } else {
        uint64_t fhi = bits_at(g_mant, s, g_sh - 64);
        uint64_t flo = bits_at(g_mant, s, g_sh - 128);
        int less = fhi < g_D[3] || (fhi == g_D[3] && flo < g_D[2]);
        count = g_D[4] + (uint64_t)less;
    }

    if (count > 0) {
        long intbits = 64 * s - g_sh;
        long flen = (intbits + 63) / 64;
        uint64_t *F;
        if (flen < 1) flen = 1;
        F = (uint64_t *)malloc((size_t)flen * sizeof(uint64_t));
        for (i = 0; i < flen; i++) F[i] = bits_at(g_mant, s, g_sh + 64 * i);
        if (g_nb == g_bcap) {
            g_bcap = g_bcap ? 2 * g_bcap : 64;
            g_F = (uint64_t **)realloc(g_F, (size_t)g_bcap * sizeof(uint64_t *));
            g_Flen = (long *)realloc(g_Flen, (size_t)g_bcap * sizeof(long));
            g_cnt = (uint64_t *)realloc(g_cnt, (size_t)g_bcap * sizeof(uint64_t));
            g_cum = (uint64_t *)realloc(g_cum, (size_t)g_bcap * sizeof(uint64_t));
        }
        g_total += count;
        g_F[g_nb] = F;
        g_Flen[g_nb] = flen;
        g_cnt[g_nb] = count;
        g_cum[g_nb] = g_total;
        g_nb++;
        g_D[4] = 0;
        g_D[3] = bits_at(g_mant, s, g_sh - 64);
        g_D[2] = bits_at(g_mant, s, g_sh - 128);
        g_D[1] = bits_at(g_mant, s, g_sh - 192);
        g_D[0] = bits_at(g_mant, s, g_sh - 256);
    }
}

seq_t a(idx_t n)
{
    long lo, hi, b, i;
    int sft;
    uint64_t before, delta;
    seq_t v;

    if (n < 1) n = 1;
    while (g_total < (uint64_t)n) {
        if (g_cap == 0 || g_m >= g_cap) {
            long want = (long)(n + n / 3 + 64);
            long nc = 2 * g_cap;
            if (nc < want) nc = want;
            init_state(nc);
        }
        step();
    }

    lo = 0; hi = g_nb - 1;
    while (lo < hi) {
        long mid = (lo + hi) / 2;
        if (g_cum[mid] >= (uint64_t)n) hi = mid; else lo = mid + 1;
    }
    b = lo;
    before = b > 0 ? g_cum[b - 1] : 0;
    delta = g_cnt[b] - ((uint64_t)n - before);

    v = 0;
    for (i = g_Flen[b] - 1; i >= 0; i--) {
        for (sft = 56; sft >= 0; sft -= 8) {
            v = v * (seq_t)256 + (seq_t)((g_F[b][i] >> sft) & 255);
        }
    }
    v = v - (seq_t)delta;
    return v;
}
