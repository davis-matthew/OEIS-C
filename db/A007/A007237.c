/*
 * A007237  Number of triangles with integer sides and area = n times
 *          perimeter.
 * https://oeis.org/A007237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. With x=s-a, y=s-b, z=s-c (s = semiperimeter), area =
 *            n*perimeter means inradius 2n, i.e. x*y*z = 4n^2*(x+y+z) with
 *            x>=y>=z>=1 integers. For each z: (y*z-m)*(x*z-m) = m*(z^2+m), m
 *            = 4n^2; enumerate divisors d <= sqrt
 * Range:     u8 n <= 11, u16 n <= 1889, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint64_t g_m, g_N, g_s, g_z;
static uint64_t g_cnt;
static uint64_t g_P[64];
static int g_E[64];
static int g_np;

static void dfs_div(int i, uint64_t d)
{
    if (i == g_np) {
        uint64_t e = g_N / d;
        if ((d + g_m) % g_z == 0 && (e + g_m) % g_z == 0 && d + g_m >= g_z * g_z)
            g_cnt++;
        return;
    }
    uint64_t v = d;
    for (int k = 0; k <= g_E[i]; k++) {
        dfs_div(i + 1, v);
        if (k < g_E[i]) {
            if (v > g_s / g_P[i]) break;
            v *= g_P[i];
            if (v > g_s) break;
        }
    }
}

static void add_factor(uint64_t p, int e)
{
    for (int i = 0; i < g_np; i++)
        if (g_P[i] == p) { g_E[i] += e; return; }
    g_P[g_np] = p; g_E[g_np] = e; g_np++;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t m = 4 * nn * nn;
    g_m = m;
    g_cnt = 0;

    /* factor n */
    uint64_t np_[64]; int ne_[64]; int nnp = 0;
    {
        uint64_t t = nn;
        for (uint64_t p = 2; p * p <= t; p++) {
            if (t % p == 0) {
                int e = 0;
                while (t % p == 0) { t /= p; e++; }
                np_[nnp] = p; ne_[nnp] = e; nnp++;
            }
        }
        if (t > 1) { np_[nnp] = t; ne_[nnp] = 1; nnp++; }
    }

    /* primes up to 4n+2 that are 1 mod 4 */
    uint64_t lim = 4 * nn + 2;
    char *comp = (char *)calloc(lim + 1, 1);
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * (lim / 2 + 2));
    uint64_t npr = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            if (i % 4 == 1) pr[npr++] = i;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }

    for (uint64_t z = 1; z * z <= 3 * m; z++) {
        uint64_t q = z * z + m;
        g_np = 0;
        /* factors of m = 4 n^2 */
        add_factor(2, 2);
        for (int i = 0; i < nnp; i++) add_factor(np_[i], 2 * ne_[i]);
        /* factor q */
        {
            int e = 0;
            while ((q & 1) == 0) { q >>= 1; e++; }
            if (e) add_factor(2, e);
            for (int i = 0; i < nnp; i++) {
                uint64_t p = np_[i];
                if (p == 2) continue;
                e = 0;
                while (q % p == 0) { q /= p; e++; }
                if (e) add_factor(p, e);
            }
            for (uint64_t k = 0; k < npr; k++) {
                uint64_t p = pr[k];
                if (p * p > q) break;
                if (q % p == 0) {
                    e = 0;
                    while (q % p == 0) { q /= p; e++; }
                    add_factor(p, e);
                }
            }
            if (q > 1) add_factor(q, 1);
        }
        g_N = m * (z * z + m);
        uint64_t s = (uint64_t)sqrt((double)g_N);
        while (s > 0 && s * s > g_N) s--;
        while ((s + 1) * (s + 1) <= g_N) s++;
        g_s = s;
        g_z = z;
        dfs_div(0, 1);
    }

    free(comp);
    free(pr);
    return (seq_t)g_cnt;
}
