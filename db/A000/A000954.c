/*
 * A000954  Conjecturally largest even integer which is an unordered sum of
 *          two primes in exactly n ways.
 * https://oeis.org/A000954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   sieve search. r(N) = number of unordered prime pairs p+q = N,
 *            computed for every even N <= L at once by squaring the odd-prime
 *            indicator polynomial with an exact NTT (mod 998244353). L is
 *            doubled until every even N in [L/2, L] has r(N) > n. a(n) =
 *            largest even N <= L with r(N) = n
 * Range:     u8 n <= 5, u16 n <= 406, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A954_MOD 998244353ULL

static uint32_t *a954_r = NULL;   /* a954_r[h] = r(2h), h = 1..a954_L/2 */
static uint64_t a954_L = 0;
static uint32_t a954_min = 0;     /* min r(2h) over h in [L/4, L/2] */

static uint64_t a954_pw(uint64_t b, uint64_t e) {
    uint64_t r = 1;
    b %= A954_MOD;
    while (e) {
        if (e & 1) r = r * b % A954_MOD;
        b = b * b % A954_MOD;
        e >>= 1;
    }
    return r;
}

static void a954_ntt(uint32_t *a, size_t m, int inv) {
    size_t i, j, k, len;
    for (i = 1, j = 0; i < m; i++) {
        size_t bit = m >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { uint32_t t = a[i]; a[i] = a[j]; a[j] = t; }
    }
    uint64_t *tw = (uint64_t *)malloc((m / 2 + 1) * sizeof(uint64_t));
    for (len = 2; len <= m; len <<= 1) {
        uint64_t w = a954_pw(3, (A954_MOD - 1) / len);
        if (inv) w = a954_pw(w, A954_MOD - 2);
        size_t half = len >> 1;
        tw[0] = 1;
        for (k = 1; k < half; k++) tw[k] = tw[k - 1] * w % A954_MOD;
        for (i = 0; i < m; i += len) {
            for (k = 0; k < half; k++) {
                uint64_t u = a[i + k];
                uint64_t v = (uint64_t)a[i + k + half] * tw[k] % A954_MOD;
                uint64_t s = u + v;
                if (s >= A954_MOD) s -= A954_MOD;
                a[i + k] = (uint32_t)s;
                a[i + k + half] = (uint32_t)(u >= v ? u - v : u + A954_MOD - v);
            }
        }
    }
    free(tw);
    if (inv) {
        uint64_t im = a954_pw(m, A954_MOD - 2);
        for (i = 0; i < m; i++) a[i] = (uint32_t)((uint64_t)a[i] * im % A954_MOD);
    }
}

static void a954_build(uint64_t L) {
    size_t m = (size_t)L, half = (size_t)(L / 2), i, j;
    unsigned char *comp = (unsigned char *)calloc(m + 1, 1);
    for (i = 2; i * i <= m; i++)
        if (!comp[i])
            for (j = i * i; j <= m; j += i) comp[j] = 1;
    uint32_t *c = (uint32_t *)calloc(m, sizeof(uint32_t));
    unsigned char *ind = (unsigned char *)calloc(half, 1);
    for (i = 0; i < half; i++) {
        size_t p = 2 * i + 1;
        if (p >= 3 && !comp[p]) { c[i] = 1; ind[i] = 1; }
    }
    free(comp);
    a954_ntt(c, m, 0);
    for (i = 0; i < m; i++) c[i] = (uint32_t)((uint64_t)c[i] * c[i] % A954_MOD);
    a954_ntt(c, m, 1);

    free(a954_r);
    a954_r = (uint32_t *)calloc(half + 1, sizeof(uint32_t));
    a954_r[1] = 0;            /* N = 2 */
    if (half >= 2) a954_r[2] = 1; /* N = 4 = 2 + 2 */
    for (size_t h = 3; h <= half; h++) {
        size_t k = h - 1;     /* N = 2h = 2(k+1) */
        uint32_t cnt = c[k];
        if ((k & 1) == 0 && ind[k / 2]) cnt++;
        a954_r[h] = cnt / 2;
    }
    free(c);
    free(ind);

    uint32_t mn = 0xFFFFFFFFu;
    for (size_t h = half / 2; h <= half; h++)
        if (h >= 1 && a954_r[h] < mn) mn = a954_r[h];
    a954_min = mn;
    a954_L = L;
}

seq_t a(idx_t n) {
    if (a954_L == 0) a954_build(4096);
    while ((uint64_t)n >= (uint64_t)a954_min && a954_L < (1ULL << 24))
        a954_build(a954_L * 2);
    uint64_t half = a954_L / 2;
    for (uint64_t h = half; h >= 1; h--) {
        if ((uint64_t)a954_r[h] == (uint64_t)n) return (seq_t)h * 2;
    }
    return (seq_t)0;
}
