/*
 * A003139  Number of coprime chains with largest member n.
 * https://oeis.org/A003139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   no closed form; computed by a dynamic program over the primes <=
 *            n, taken in descending order. Each prime p either already
 *            divides a chosen member or starts a new member m <= n whose
 *            largest prime factor is p (n itself is forced). The state is the
 *            set of smaller primes already covered. a(n) is th...
 * Range:     u8 n <= 46, u16 n <= 156, u32 n <= 299, u64 n <= 299,
 *            u128 n <= 299, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HCAP (1u << 20)
#define EMPTYK UINT64_MAX
#define MEMOSZ 40000

static uint64_t hk[2][HCAP];
static seq_t hv[2][HCAP];
static uint32_t hused[2][HCAP];
static uint32_t hcnt[2];
static bool hinit = false;

static seq_t memo[MEMOSZ];
static unsigned char have[MEMOSZ];

static void h_add(int t, uint64_t key, seq_t v) {
    uint32_t h = (uint32_t)((key * 0x9E3779B97F4A7C15ULL) >> 44) & (HCAP - 1);
    while (hk[t][h] != EMPTYK && hk[t][h] != key) h = (h + 1) & (HCAP - 1);
    if (hk[t][h] == EMPTYK) {
        hk[t][h] = key;
        hv[t][h] = v;
        hused[t][hcnt[t]++] = h;
    } else {
        hv[t][h] += v;
    }
}

static void h_clear(int t) {
    uint32_t i;
    for (i = 0; i < hcnt[t]; i++) {
        uint32_t h = hused[t][i];
        hk[t][h] = EMPTYK;
        hv[t][h] = 0;
    }
    hcnt[t] = 0;
}

static seq_t compute(idx_t n) {
    size_t N, i, j;
    uint32_t *spf;
    int *pidx, *lp, *cnt, *off, *fill;
    uint64_t *mk, *ch;
    int np = 0, j0, cur;
    long jj;
    uint64_t mask0;
    seq_t result;

    if (n < 2) return (seq_t)1;
    if (!hinit) {
        memset(hk, 0xFF, sizeof(hk));
        hcnt[0] = hcnt[1] = 0;
        hinit = true;
    }
    N = (size_t)n;
    spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    pidx = (int *)malloc((N + 1) * sizeof(int));
    lp = (int *)malloc((N + 1) * sizeof(int));
    mk = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            pidx[i] = np++;
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= N; i++) {
        size_t x = i, last = 0;
        uint64_t mask = 0;
        while (x > 1) {
            size_t p = spf[x];
            while (x % p == 0) x /= p;
            if (last) {
                int b = pidx[last];
                if (b < 64) mask |= (uint64_t)1 << b;
            }
            last = p;
        }
        lp[i] = pidx[last];
        mk[i] = mask;
    }
    cnt = (int *)calloc((size_t)np + 1, sizeof(int));
    off = (int *)calloc((size_t)np + 1, sizeof(int));
    fill = (int *)calloc((size_t)np + 1, sizeof(int));
    for (i = 2; i < N; i++) cnt[lp[i]]++;
    for (jj = 0; jj < np; jj++) off[jj + 1] = off[jj] + cnt[jj];
    ch = (uint64_t *)malloc(((size_t)off[np] + 1) * sizeof(uint64_t));
    for (i = 2; i < N; i++) {
        int b = lp[i];
        ch[off[b] + fill[b]++] = mk[i];
    }
    j0 = lp[N];
    mask0 = mk[N];

    cur = 0;
    h_clear(0);
    h_clear(1);
    h_add(0, 0, (seq_t)1);
    for (jj = np - 1; jj >= 0; jj--) {
        int nt = cur ^ 1;
        uint64_t bitj = (jj < 64) ? ((uint64_t)1 << jj) : 0;
        uint32_t u;
        for (u = 0; u < hcnt[cur]; u++) {
            uint32_t h = hused[cur][u];
            uint64_t key = hk[cur][h];
            seq_t v = hv[cur][h];
            if (jj == j0) {
                if (bitj && (key & bitj)) continue;
                if ((key & mask0) == 0) h_add(nt, key | mask0, v);
            } else if (bitj && (key & bitj)) {
                h_add(nt, key & ~bitj, v);
            } else {
                int k;
                for (k = off[jj]; k < off[jj + 1]; k++) {
                    uint64_t c = ch[k];
                    if ((key & c) == 0) h_add(nt, key | c, v);
                }
            }
        }
        h_clear(cur);
        cur = nt;
    }
    result = 0;
    {
        uint32_t u;
        for (u = 0; u < hcnt[cur]; u++) {
            uint32_t h = hused[cur][u];
            if (hk[cur][h] == 0) result += hv[cur][h];
        }
    }
    h_clear(cur);

    free(spf); free(pidx); free(lp); free(mk);
    free(cnt); free(off); free(fill); free(ch);
    return result;
}

seq_t a(idx_t n) {
    seq_t r;
    if (n < MEMOSZ && have[n]) return memo[n];
    r = compute(n);
    if (n < MEMOSZ) {
        memo[n] = r;
        have[n] = 1;
    }
    return r;
}
