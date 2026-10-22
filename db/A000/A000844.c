/*
 * A000844  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count of maps {0,1}^n -> {0,1}^2 up to the
 *            hyperoctahedral group B_n on the domain and XOR on the range:
 *            a(n) = (1/(4*2^n*n!)) * Sum_{g in B_n} 4^cyc(g) * (1 + 3*[all
 *            cycles of g on {0,1}^n have even length]), where cyc(g) is the
 *            number of cycles of g acting on the 2^n points; computed b...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   add_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/add_at.h"

static int g_n;
static uint32_t *g_S;
static size_t g_L;
static int g_keys[64];
static unsigned char *g_vis;
static uint64_t g_tot;

static void process(int cnt)
{
    int perm[64], flip[64];
    int pos = 0, t, j, run = 1;
    uint64_t z = 1, size, N, x;
    uint64_t cyc = 0;
    int even = 1;
    for (t = 0; t < cnt; t++) {
        int k = g_keys[t] >> 1, s = g_keys[t] & 1;
        for (j = 0; j < k; j++) {
            perm[pos + j] = pos + (j + 1) % k;
            flip[pos + j] = 0;
        }
        if (s) flip[pos + k - 1] = 1;
        pos += k;
        z *= (uint64_t)(2 * k);
        if (t > 0 && g_keys[t] == g_keys[t - 1]) run++;
        else run = 1;
        z *= (uint64_t)run;
    }
    size = g_tot / z;
    N = (uint64_t)1 << g_n;
    memset(g_vis, 0, (size_t)N);
    for (x = 0; x < N; x++) {
        uint64_t y, len = 0;
        if (g_vis[x]) continue;
        y = x;
        while (!g_vis[y]) {
            uint64_t img = 0;
            int i;
            g_vis[y] = 1;
            len++;
            for (i = 0; i < g_n; i++)
                if ((((y >> i) & 1) ^ (uint64_t)flip[i]) != 0)
                    img |= (uint64_t)1 << perm[i];
            y = img;
        }
        cyc++;
        if (len & 1) even = 0;
    }
    add_at(g_S, g_L, size, (size_t)(2 * cyc));
    if (even) {
        add_at(g_S, g_L, size, (size_t)(2 * cyc));
        add_at(g_S, g_L, size, (size_t)(2 * cyc));
        add_at(g_S, g_L, size, (size_t)(2 * cyc));
    }
}

static void rec(int rem, int maxkey, int cnt)
{
    int key;
    if (rem == 0) {
        process(cnt);
        return;
    }
    for (key = maxkey; key >= 2; key--) {
        int k = key >> 1;
        if (k > rem) continue;
        g_keys[cnt] = key;
        rec(rem - k, key, cnt + 1);
    }
}

seq_t a(idx_t n)
{
    uint64_t N, rem;
    size_t i, L;
    int j, sh;
    seq_t r = 0;
    if (n < 1 || n > 16) return r;
    g_n = (int)n;
    N = (uint64_t)1 << g_n;
    L = (size_t)((2 * N + 160) / 32 + 2);
    g_L = L;
    g_S = (uint32_t *)calloc(L, sizeof(uint32_t));
    g_vis = (unsigned char *)malloc((size_t)N);
    g_tot = (uint64_t)1 << g_n;
    for (j = 2; j <= g_n; j++) g_tot *= (uint64_t)j;
    rec(g_n, 2 * g_n + 1, 0);

    /* divide by 2^(n+2) */
    sh = g_n + 2;
    while (sh > 0) {
        int s = sh > 31 ? 31 : sh;
        for (i = 0; i < L; i++) {
            uint32_t hi = (i + 1 < L) ? g_S[i + 1] : 0;
            g_S[i] = (g_S[i] >> s) | (uint32_t)((uint64_t)hi << (32 - s));
        }
        sh -= s;
    }
    /* divide by n! */
    for (j = 2; j <= g_n; j++) {
        rem = 0;
        for (i = L; i-- > 0;) {
            uint64_t cur = (rem << 32) | g_S[i];
            g_S[i] = (uint32_t)(cur / (uint64_t)j);
            rem = cur % (uint64_t)j;
        }
    }
    for (i = L; i-- > 0;) {
        r = r * 65536u + (seq_t)(g_S[i] >> 16);
        r = r * 65536u + (seq_t)(g_S[i] & 0xffffu);
    }
    free(g_S);
    free(g_vis);
    return r;
}
