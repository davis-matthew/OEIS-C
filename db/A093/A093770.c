/*
 * A093770  Non-perfect powers k for which q = A051903(k)/A051904(k) is an
 *          integer, A051904(k) > 1.
 * https://oeis.org/A093770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate powerful numbers k <= L (all prime exponents
 *            >= 2) by DFS, keep those with gcd of exponents = 1
 *            (non-perfect-power) and (max exponent) % (min exponent) == 0;
 *            sort, double L until n terms are found
 * Range:     u8 none, u16 n <= 10, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 47 terms through n = 9993 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A093770_res = 0;
static size_t A093770_cnt = 0, A093770_cap = 0;
static uint64_t *A093770_pr = 0;
static size_t A093770_np = 0;
static uint64_t A093770_lim = 0;

static unsigned A093770_g(unsigned a, unsigned b)
{
    while (b) { unsigned t = a % b; a = b; b = t; }
    return a;
}

static int A093770_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static void A093770_dfs(size_t idx, uint64_t cur, unsigned mn, unsigned mx, unsigned g)
{
    for (size_t i = idx; i < A093770_np; ++i) {
        uint64_t p = A093770_pr[i];
        if (p * p > A093770_lim / cur) break;
        uint64_t pe = p;
        unsigned e = 1;
        uint64_t room = A093770_lim / cur;
        for (;;) {
            if (pe > room / p) break;
            pe *= p;
            ++e;
            uint64_t nv = cur * pe;
            unsigned nmn = (cur == 1 || e < mn) ? e : mn;
            unsigned nmx = (cur == 1 || e > mx) ? e : mx;
            unsigned ng = (cur == 1) ? e : A093770_g(g, e);
            if (ng == 1 && nmn > 1 && nmx % nmn == 0) {
                if (A093770_cnt == A093770_cap) {
                    A093770_cap = A093770_cap ? A093770_cap * 2 : 1024;
                    A093770_res = (uint64_t *)realloc(A093770_res, A093770_cap * sizeof(uint64_t));
                }
                A093770_res[A093770_cnt++] = nv;
            }
            A093770_dfs(i + 1, nv, nmn, nmx, ng);
        }
    }
}

static void A093770_build(uint64_t lim)
{
    uint64_t s = 1;
    while ((s + 1) * (s + 1) <= lim) ++s;
    free(A093770_pr);
    A093770_pr = (uint64_t *)malloc((s + 2) * sizeof(uint64_t));
    char *comp = (char *)calloc(s + 2, 1);
    A093770_np = 0;
    for (uint64_t i = 2; i <= s; ++i) {
        if (!comp[i]) {
            A093770_pr[A093770_np++] = i;
            for (uint64_t j = i * i; j <= s; j += i) comp[j] = 1;
        }
    }
    free(comp);
    A093770_cnt = 0;
    A093770_lim = lim;
    A093770_dfs(0, 1, 0, 0, 0);
    qsort(A093770_res, A093770_cnt, sizeof(uint64_t), A093770_cmp);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (A093770_lim == 0) A093770_build((uint64_t)1 << 20);
    while ((uint64_t)A093770_cnt < (uint64_t)n)
        A093770_build(A093770_lim * 4);
    return (seq_t)A093770_res[n - 1];
}
