/*
 * A002956  Number of basic invariants for cyclic group of order and degree n.
 * https://oeis.org/A002956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A096337(n) + 1. Computed by search: count the minimal
 *            zero-sum multisets over Z_n. Do a DFS over zero-sum-free
 *            multisets S (elements in nondecreasing order), keeping a bitset
 *            of subset sums. Each S whose completing element g = -sum(S) mod
 *            n is >= max(S) gives one minimal zero-sum multiset...
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int a2956_N, a2956_W;
static uint64_t *a2956_buf;
static uint64_t a2956_cnt;

static void a2956_rot(const uint64_t *R, uint64_t *out, int x)
{
    int W = a2956_W, N = a2956_N, i;
    int ws = x / 64, bs = x % 64;
    int y = N - x;
    int ws2 = y / 64, bs2 = y % 64;
    for (i = 0; i < W; i++) {
        uint64_t v = 0;
        int j = i - ws;
        if (j >= 0) v |= R[j] << bs;
        if (bs && j - 1 >= 0) v |= R[j - 1] >> (64 - bs);
        j = i + ws2;
        if (j < W) v |= R[j] >> bs2;
        if (bs2 && j + 1 < W) v |= R[j + 1] << (64 - bs2);
        out[i] = v;
    }
    if (N % 64) out[W - 1] &= (((uint64_t)1) << (N % 64)) - 1;
}

static void a2956_dfs(int depth, int last, int sigma)
{
    int N = a2956_N, W = a2956_W, x, i;
    uint64_t *R = a2956_buf + (size_t)depth * W;
    uint64_t *R2 = a2956_buf + (size_t)(depth + 1) * W;
    for (x = last; x < N; x++) {
        int nx = N - x;
        int s2, g;
        if ((R[nx / 64] >> (nx % 64)) & 1) continue;
        a2956_rot(R, R2, x);
        for (i = 0; i < W; i++) R2[i] |= R[i];
        s2 = (sigma + x) % N;
        g = (N - s2) % N;
        if (g >= x) a2956_cnt++;
        a2956_dfs(depth + 1, x, s2);
    }
}

static uint64_t a2956_compute(int n)
{
    a2956_N = n;
    a2956_W = (n + 63) / 64;
    a2956_buf = (uint64_t *)calloc((size_t)(n + 2) * a2956_W, sizeof(uint64_t));
    a2956_buf[0] = 1;
    a2956_cnt = 0;
    a2956_dfs(0, 1, 0);
    free(a2956_buf);
    a2956_buf = NULL;
    return a2956_cnt + 1;
}

static uint64_t *a2956_memo = NULL;
static idx_t a2956_memolen = 0;

seq_t a(idx_t n)
{
    uint64_t c;
    seq_t r;
    uint32_t hi, lo;
    if (n < 1) return 0;
    if (n >= a2956_memolen) {
        idx_t newlen = n + 1, i;
        uint64_t *p = (uint64_t *)realloc(a2956_memo, (size_t)newlen * sizeof(uint64_t));
        if (!p) return 0;
        for (i = a2956_memolen; i < newlen; i++) p[i] = 0;
        a2956_memo = p;
        a2956_memolen = newlen;
    }
    if (a2956_memo[n] == 0) a2956_memo[n] = a2956_compute((int)n);
    c = a2956_memo[n];
    hi = (uint32_t)(c >> 32);
    lo = (uint32_t)(c & 0xFFFFFFFFu);
    if (hi == 0) return (seq_t)lo;
    r = (seq_t)hi;
    r <<= 32;
    r += (seq_t)lo;
    return r;
}
