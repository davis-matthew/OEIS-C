/*
 * A000829  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2^(n+1) n!)) * Sum_{g in B_n} (4^c(g) + 4^ce(g) *
 *            2^co(g)), where B_n is the hyperoctahedral group acting on
 *            {0,1}^n, c(g) is its number of cycles on the 2^n points, and
 *            ce(g)/co(g) count the even/odd-length cycles (Burnside over B_n
 *            x S_2 acting on pairs of Boolean functions); summed...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a829_add, a829_addbit, a829_div, a829_gcd, a829_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a829_add.h"
#include "../utils/a829_addbit.h"
#include "../utils/a829_div.h"
#include "../utils/a829_gcd.h"
#include "../utils/a829_mul.h"

static int a829_n;
static int *a829_mp, *a829_mn;
static uint32_t *a829_acc, *a829_tmp;
static size_t a829_L;

/* number of points of {0,1}^n fixed by g^j */
static uint64_t a829_fix(uint64_t j)
{
    int n = a829_n, k;
    uint64_t e = 0;
    for (k = 1; k <= n; k++) {
        if (a829_mn[k]) {
            uint64_t g = a829_gcd(j, 2 * (uint64_t)k);
            if (((2 * (uint64_t)k / g) & 1) == 0) return 0;
            e += (uint64_t)a829_mn[k] * (g / 2);
        }
        if (a829_mp[k])
            e += (uint64_t)a829_mp[k] * a829_gcd(j, (uint64_t)k);
    }
    return (uint64_t)1 << e;
}

static void a829_process(void)
{
    int n = a829_n, k, i;
    uint64_t L = 1, Lo, j, s, c, co, ce;
    size_t LL = a829_L;
    for (k = 1; k <= n; k++) {
        if (a829_mp[k]) L = L / a829_gcd(L, (uint64_t)k) * (uint64_t)k;
        if (a829_mn[k]) L = L / a829_gcd(L, 2 * (uint64_t)k) * (2 * (uint64_t)k);
    }
    s = 0;
    for (j = 1; j <= L; j++) s += a829_fix(j);
    c = s / L;
    Lo = L;
    while ((Lo & 1) == 0) Lo >>= 1;
    s = 0;
    for (j = 1; j <= Lo; j++) s += a829_fix(a829_gcd(j, Lo));
    co = s / Lo;
    ce = c - co;

    memset(a829_tmp, 0, LL * sizeof(uint32_t));
    a829_addbit(a829_tmp, LL, 2 * c);
    a829_addbit(a829_tmp, LL, 2 * ce + co);
    for (i = 1; i <= n; i++) a829_mul(a829_tmp, LL, (uint32_t)(2 * i));
    for (k = 1; k <= n; k++) {
        int t = a829_mp[k] + a829_mn[k];
        for (i = 0; i < t; i++) a829_div(a829_tmp, LL, (uint32_t)(2 * k));
        for (i = 2; i <= a829_mp[k]; i++) a829_div(a829_tmp, LL, (uint32_t)i);
        for (i = 2; i <= a829_mn[k]; i++) a829_div(a829_tmp, LL, (uint32_t)i);
    }
    a829_add(a829_acc, a829_tmp, LL);
}

static void a829_rec(int k, int rem)
{
    int p, q;
    if (k == 0) {
        if (rem == 0) a829_process();
        return;
    }
    for (p = 0; p * k <= rem; p++) {
        for (q = 0; (p + q) * k <= rem; q++) {
            if (k == 1 && p + q != rem) continue;
            a829_mp[k] = p;
            a829_mn[k] = q;
            a829_rec(k - 1, rem - (p + q) * k);
        }
    }
    a829_mp[k] = 0;
    a829_mn[k] = 0;
}

seq_t a(idx_t n)
{
    int nn, i;
    size_t LL, t;
    seq_t r = 0;
    if (n < 1 || n > 24) return 0;
    nn = (int)n;
    a829_n = nn;
    LL = (size_t)((((uint64_t)1 << (nn + 1)) + 2 + 8 * (uint64_t)nn + 64) / 32 + 2);
    a829_L = LL;
    a829_mp = (int *)calloc((size_t)nn + 1, sizeof(int));
    a829_mn = (int *)calloc((size_t)nn + 1, sizeof(int));
    a829_acc = (uint32_t *)calloc(LL, sizeof(uint32_t));
    a829_tmp = (uint32_t *)calloc(LL, sizeof(uint32_t));

    a829_rec(nn, nn);

    a829_div(a829_acc, LL, 2);
    for (i = 1; i <= nn; i++) a829_div(a829_acc, LL, (uint32_t)(2 * i));

    t = LL;
    while (t > 0) {
        int b;
        t--;
        for (b = 3; b >= 0; b--) {
            r <<= 8;
            r += (unsigned)((a829_acc[t] >> (8 * b)) & 0xFFu);
        }
    }

    free(a829_mp);
    free(a829_mn);
    free(a829_acc);
    free(a829_tmp);
    return r;
}
