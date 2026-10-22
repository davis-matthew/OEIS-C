/*
 * A004892  Numbers that are the sum of at most 8 positive 9th powers.
 * https://oeis.org/A004892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- list every sum of at most 8 ninth powers b^9 (b >= 1)
 *            up to a bound X, sort and dedupe; double X until at least n
 *            terms exist
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   release, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zalloc_arr.h"
#include "../utils/release.h"

static seq_t g_X;
static seq_t pw[256];
static int npw = 0;
static seq_t *buf = NULL;
static idx_t cnt = 0;

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_alloc = 0;
static seq_t cache_X;
static int inited = 0;

static void dfs(seq_t cur, int k, int maxb, int fill)
{
    if (fill) buf[cnt] = cur;
    cnt++;
    if (k == 0) return;
    for (int b = 1; b <= maxb; b++) {
        if (pw[b] > g_X - cur) break;
        dfs(cur + pw[b], k - 1, b, fill);
    }
}

static void sift(seq_t *v, idx_t i, idx_t len)
{
    for (;;) {
        idx_t l = 2 * i + 1, m = i;
        if (l < len && v[l] > v[m]) m = l;
        if (l + 1 < len && v[l + 1] > v[m]) m = l + 1;
        if (m == i) return;
        seq_t t = v[i]; v[i] = v[m]; v[m] = t;
        i = m;
    }
}

static void heap_sort(seq_t *v, idx_t len)
{
    if (len < 2) return;
    for (idx_t i = len / 2; i-- > 0; ) sift(v, i, len);
    for (idx_t end = len - 1; end > 0; end--) {
        seq_t t = v[0]; v[0] = v[end]; v[end] = t;
        sift(v, 0, end);
    }
}

static void build(seq_t X)
{
    g_X = X;
    npw = 0;
    for (int b = 1; b < 255; b++) {
        seq_t p = 1;
        int ok = 1;
        for (int i = 0; i < 9; i++) {
            if (p > X / (seq_t)b) { ok = 0; break; }
            p *= (seq_t)b;
        }
        if (!ok) break;
        pw[++npw] = p;
    }
    cnt = 0;
    dfs((seq_t)0, 8, npw, 0);
    idx_t total = cnt;
    buf = zalloc_arr(total);
    cnt = 0;
    dfs((seq_t)0, 8, npw, 1);
    heap_sort(buf, total);
    idx_t u = 0;
    for (idx_t i = 0; i < total; i++) {
        if (u == 0 || buf[i] != buf[u - 1]) {
            if (u != i) buf[u] = buf[i];
            u++;
        }
    }
    if (cache != NULL) release(cache, cache_alloc);
    cache = buf;
    cache_alloc = total;
    cache_len = u;
    cache_X = X;
    buf = NULL;
}

seq_t a(idx_t n)
{
    while (!inited || cache_len < n) {
        seq_t X;
        if (!inited) {
            X = (seq_t)64;
        } else {
            X = cache_X * 2;
            if (X <= cache_X) break;  /* fixed-width overflow: cannot grow */
        }
        build(X);
        inited = 1;
    }
    if (n > cache_len) return cache_len ? cache[cache_len - 1] : (seq_t)0;
    return cache[n - 1];
}
