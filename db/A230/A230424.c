/*
 * A230424  a(n) = largest natural number x such that x=n+A034968(x), or zero
 *          if no such number exists.
 * https://oeis.org/A230424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest x with x = n + A034968(x), or 0 if none; computed
 *            by DFS over factorial-base digits d_k<=k with n = Sum_{k>=2}
 *            d_k*(k!-1), x = 1 + Sum d_k*k!
 * Range:     u8 n <= 249, u16 n <= 10079, u32 n <= 10079, u64 n <= 10079,
 *            u128 n <= 10079, big any n
 * Validated: standard, pass, 91 terms through n = 9627 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A230424_w;   /* k! - 1 */
static seq_t *A230424_f;   /* k! */
static seq_t *A230424_ml;  /* capped max reachable with weights 2..k */

static bool A230424_dfs(int k, seq_t rem, seq_t x, seq_t *best)
{
    if (k < 2) {
        if (rem == 0) { *best = x; return true; }
        return false;
    }
    bool found = false;
    seq_t w = A230424_w[k];
    seq_t dmax = rem / w;
    if (dmax > (seq_t)k) dmax = (seq_t)k;
    for (long long di = (long long)dmax; di >= 0; --di) {
        seq_t d = (seq_t)di;
        seq_t r = rem - d * w;
        if (r > A230424_ml[k - 1]) break;   /* smaller d only increases r */
        seq_t b;
        if (A230424_dfs(k - 1, r, x + d * A230424_f[k], &b)) {
            if (!found || b > *best) { *best = b; found = true; }
        }
    }
    return found;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* find K = largest k with k!-1 <= sn */
    int K = 1;
    {
        seq_t f = 1;
        while (true) {
            seq_t div = (seq_t)(K + 1);
            if (f > (sn + 1) / div) break;
            f *= div;
            K++;
        }
    }
    A230424_w  = mem_new(seq_t, K + 2);
    A230424_f  = mem_new(seq_t, K + 2);
    A230424_ml = mem_new(seq_t, K + 2);
    A230424_f[1] = 1;
    A230424_w[1] = 0;
    A230424_ml[1] = 0;
    for (int k = 2; k <= K; ++k) {
        A230424_f[k] = A230424_f[k - 1] * (seq_t)k;
        A230424_w[k] = A230424_f[k] - 1;
        seq_t ml = A230424_ml[k - 1];
        if (ml >= sn || A230424_w[k] > sn / (seq_t)k) {
            ml = sn;
        } else {
            seq_t t = (seq_t)k * A230424_w[k];
            if (t >= sn - ml) ml = sn; else ml += t;
        }
        A230424_ml[k] = ml;
    }
    seq_t best = 0;
    bool ok = A230424_dfs(K, sn, 0, &best);
    seq_t res = ok ? best + 1 : (seq_t)0;
    mem_free(A230424_w);
    mem_free(A230424_f);
    mem_free(A230424_ml);
    return res;
}
