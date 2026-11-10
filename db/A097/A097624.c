/*
 * A097624  Smallest m such that A097621(m) = n.
 * https://oeis.org/A097624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with A097621(m) = n. A097621 is multiplicative
 *            with A097621(p^e) = index of p^e in A000961, so write n as a
 *            product of distinct factors k_i >= 2 whose prime powers
 *            A000961(k_i) have distinct primes, and minimize the product of
 *            the A000961(k_i). This is done by a DFS search
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static idx_t *pp_tab = NULL;   /* pp_tab[k] = A000961(k), k>=1 */
static idx_t pp_cnt = 0;

static void build_pp(idx_t need)
{
    if (pp_tab && pp_cnt >= need) return;
    if (pp_tab) { free(pp_tab); pp_tab = NULL; }
    idx_t L = 64;
    for (;;) {
        unsigned char *f = (unsigned char*)calloc((size_t)L + 1, 1);
        unsigned char *comp = (unsigned char*)calloc((size_t)L + 1, 1);
        idx_t cnt = 1;
        f[1] = 1;
        for (idx_t i = 2; i <= L; i++) {
            if (!comp[i]) {
                for (idx_t j = i * i; j <= L && i <= L / i; j += i) comp[j] = 1;
                idx_t v = i;
                for (;;) {
                    f[v] = 1; cnt++;
                    if (v > L / i) break;
                    v *= i;
                }
            }
        }
        if (cnt >= need) {
            pp_tab = (idx_t*)malloc(((size_t)cnt + 1) * sizeof(idx_t));
            idx_t c = 0;
            for (idx_t i = 1; i <= L; i++) if (f[i]) pp_tab[++c] = i;
            pp_cnt = c;
            free(f); free(comp);
            return;
        }
        free(f); free(comp);
        L *= 2;
    }
}

static idx_t spf_small(idx_t q)
{
    for (idx_t d = 2; d <= q / d; d++)
        if (q % d == 0) return d;
    return q;
}

static idx_t used_p[128];
static seq_t best_m;
static bool have_best;

static void dfs(idx_t rem, idx_t maxf, int nu, seq_t cur)
{
    if (have_best && cur >= best_m) return;
    if (rem == 1) {
        best_m = cur; have_best = true;
        return;
    }
    idx_t top = rem < maxf ? rem : maxf;
    for (idx_t d = top; d >= 2; d--) {
        if (rem % d) continue;
        idx_t q = pp_tab[d];
        idx_t p = spf_small(q);
        bool ok = true;
        for (int i = 0; i < nu; i++) if (used_p[i] == p) { ok = false; break; }
        if (!ok) continue;
        used_p[nu] = p;
        dfs(rem / d, d - 1, nu + 1, cur * (seq_t)q);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    build_pp(n);
    have_best = false;
    best_m = (seq_t)0;
    dfs(n, n, 0, (seq_t)1);
    return best_m;
}
