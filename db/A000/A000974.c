/*
 * A000974  Conjecturally the number of even integers the sum of two primes in
 *          exactly n ways.
 * https://oeis.org/A000974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   sieve-based search. Count g(m) = #{p <= q primes : p+q = m} for
 *            every even m up to L using an exact NTT self-convolution of the
 *            odd-prime indicator. L doubles until min g(m) over (L/2, L] is
 *            clearly above n. a(n) = #{even m : g(m) = n}
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define NTT_P 998244353u

static uint32_t *g_cnt = NULL;   /* g_cnt[j] = g(2j), 1 <= j <= g_L/2 */
static uint64_t g_L = 0;

static uint64_t ntt_pw(uint64_t b, uint64_t e) {
    uint64_t r = 1;
    b %= NTT_P;
    while (e) {
        if (e & 1) r = r * b % NTT_P;
        b = b * b % NTT_P;
        e >>= 1;
    }
    return r;
}

static void ntt(uint32_t *a, uint64_t N, int invert, uint32_t *wt) {
    uint64_t i, j, len;
    for (i = 1, j = 0; i < N; i++) {
        uint64_t bit = N >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { uint32_t t = a[i]; a[i] = a[j]; a[j] = t; }
    }
    for (len = 2; len <= N; len <<= 1) {
        uint64_t half = len >> 1, k;
        uint64_t wl = ntt_pw(3, (NTT_P - 1) / len);
        if (invert) wl = ntt_pw(wl, NTT_P - 2);
        uint64_t w = 1;
        for (k = 0; k < half; k++) { wt[k] = (uint32_t)w; w = w * wl % NTT_P; }
        for (i = 0; i < N; i += len) {
            uint32_t *x = a + i, *y = a + i + half;
            for (k = 0; k < half; k++) {
                uint32_t u = x[k];
                uint32_t v = (uint32_t)((uint64_t)y[k] * wt[k] % NTT_P);
                uint32_t s = u + v;
                if (s >= NTT_P) s -= NTT_P;
                x[k] = s;
                y[k] = (u >= v) ? u - v : u + NTT_P - v;
            }
        }
    }
    if (invert) {
        uint64_t ninv = ntt_pw(N, NTT_P - 2);
        for (i = 0; i < N; i++) a[i] = (uint32_t)((uint64_t)a[i] * ninv % NTT_P);
    }
}

static void build(uint64_t L) {
    if (L <= g_L) return;
    uint64_t H = L / 2, i, j;
    /* comp[i] refers to the odd number 2i+1 */
    bool *comp = (bool *)calloc(H + 1, sizeof(bool));
    comp[0] = true;
    for (i = 1; (2 * i + 1) * (2 * i + 1) <= 2 * H + 1; i++) {
        if (comp[i]) continue;
        uint64_t p = 2 * i + 1;
        for (j = (p * p - 1) / 2; j <= H; j += p) comp[j] = true;
    }
    uint32_t *f = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *wt = (uint32_t *)malloc((L / 2 + 1) * sizeof(uint32_t));
    for (i = 1; i < H; i++) if (!comp[i]) f[i] = 1;
    ntt(f, L, 0, wt);
    for (i = 0; i < L; i++) f[i] = (uint32_t)((uint64_t)f[i] * f[i] % NTT_P);
    ntt(f, L, 1, wt);
    /* f[k] = ordered count of odd-prime pairs summing to 2k+2 */
    uint32_t *cnt = (uint32_t *)calloc(H + 1, sizeof(uint32_t));
    for (j = 1; j <= H; j++) {          /* m = 2j */
        uint64_t r = f[j - 1];
        if ((j & 1) && !comp[(j - 1) / 2]) r++;  /* p = q = j */
        uint64_t g = r / 2;
        if (j == 2) g++;                 /* 4 = 2 + 2 */
        cnt[j] = (uint32_t)g;
    }
    free(f);
    free(wt);
    free(comp);
    free(g_cnt);
    g_cnt = cnt;
    g_L = L;
}

seq_t a(idx_t n) {
    uint64_t L = 1024;
    uint64_t need = (uint64_t)n + (uint64_t)n / 8 + 16;
    for (;;) {
        build(L);
        uint64_t mn = UINT64_MAX, j;
        for (j = L / 4 + 1; j <= L / 2; j++)
            if ((uint64_t)g_cnt[j] < mn) mn = g_cnt[j];
        if (mn > need) break;
        L *= 2;
    }
    uint64_t c = 0, j;
    for (j = 1; j <= g_L / 2; j++)
        if ((uint64_t)g_cnt[j] == (uint64_t)n) c++;
    seq_t r = 0;
    r += c;
    return r;
}
