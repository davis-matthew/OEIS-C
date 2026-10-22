/*
 * A004881  Numbers that are the sum of at most 8 nonzero 8th powers.
 * https://oeis.org/A004881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate all sums of at most 8 nonzero 8th powers up to
 *            a bound L (including the empty sum 0), sort and remove
 *            duplicates; raise L until at least n terms are found
 * Range:     u8 n <= 9, u16 n <= 165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_ull, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/cmp_ull.h"

static unsigned long long *cache_vals = NULL;
static idx_t cache_count = 0;

static unsigned long long *buf = NULL;
static size_t buf_len = 0, buf_cap = 0;
static unsigned long long pw[64];
static int npw = 0;
static unsigned long long lim = 0;

static void push_val(unsigned long long v) {
    if (buf_len >= buf_cap) {
        buf_cap = buf_cap ? buf_cap * 2 : 1024;
        buf = (unsigned long long *)realloc(buf, buf_cap * sizeof(unsigned long long));
    }
    buf[buf_len++] = v;
}

static void dfs(int maxb, unsigned long long sum, int left) {
    int b;
    push_val(sum);
    if (left == 0) return;
    for (b = 1; b <= maxb; b++) {
        if (pw[b] > lim - sum) break;
        dfs(b, sum + pw[b], left - 1);
    }
}

static void build(unsigned long long L) {
    size_t i, k;
    lim = L;
    npw = 0;
    pw[0] = 0;
    for (i = 1; i < 64; i++) {
        unsigned long long p = 1;
        int j, over = 0;
        for (j = 0; j < 8; j++) {
            if (p > L / i) { over = 1; break; }
            p *= i;
        }
        if (over || p > L) break;
        pw[i] = p;
        npw = (int)i;
    }
    buf_len = 0;
    dfs(npw, 0, 8);
    qsort(buf, buf_len, sizeof(unsigned long long), cmp_ull);
    k = 0;
    for (i = 0; i < buf_len; i++) {
        if (k == 0 || buf[i] != buf[k - 1]) buf[k++] = buf[i];
    }
    free(cache_vals);
    cache_vals = (unsigned long long *)malloc((k ? k : 1) * sizeof(unsigned long long));
    for (i = 0; i < k; i++) cache_vals[i] = buf[i];
    cache_count = (idx_t)k;
}

seq_t a(idx_t n) {
    if (n < 1) n = 1;
    if (cache_count < n) {
        unsigned long long L = lim ? lim : (1ULL << 16);
        while (1) {
            build(L);
            if (cache_count >= n) break;
            L *= 4;
        }
    }
    return u128_to_seq((unsigned __int128)cache_vals[n - 1]);
}
