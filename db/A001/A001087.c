/*
 * A001087  Related to S(n), the number of self-dual monotone Boolean
 *          functions of n variables (A001206): 2^n-th term is S(n).
 * https://oeis.org/A001087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   no closed form; a(n) is computed by a search. a(n) = number of
 *            intersecting antichains (the empty family included) whose
 *            members are the subsets of {1,2,...} with binary codes 0..n-1.
 *            These are the cliques of the graph on 1..n-1 where i~j iff
 *            (i&j)!=0, (i&j)!=i, (i&j)!=j. So a(2^(k-1)) = S(k). Cl...
 * Range:     u8 n <= 21, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int g_W;
static uint64_t *g_nbr;
static uint64_t *g_buf;
static uint64_t *g_keys;
static unsigned __int128 *g_vals;
static unsigned char *g_used;
static size_t g_cap, g_cnt;

static size_t a001087_hash(const uint64_t *C)
{
    uint64_t h = 0x9E3779B97F4A7C15ULL;
    int w;
    for (w = 0; w < g_W; w++) {
        h ^= C[w];
        h *= 0xBF58476D1CE4E5B9ULL;
        h ^= h >> 31;
    }
    return (size_t)h;
}

static int a001087_find(const uint64_t *C, size_t *slot)
{
    size_t m = g_cap - 1, p = a001087_hash(C) & m;
    while (g_used[p]) {
        if (memcmp(g_keys + p * (size_t)g_W, C, sizeof(uint64_t) * (size_t)g_W) == 0) {
            *slot = p;
            return 1;
        }
        p = (p + 1) & m;
    }
    *slot = p;
    return 0;
}

static void a001087_grow(void)
{
    size_t oc = g_cap, i;
    uint64_t *ok = g_keys;
    unsigned __int128 *ov = g_vals;
    unsigned char *ou = g_used;
    g_cap = oc * 2;
    g_keys = (uint64_t *)malloc(sizeof(uint64_t) * g_cap * (size_t)g_W);
    g_vals = (unsigned __int128 *)malloc(sizeof(unsigned __int128) * g_cap);
    g_used = (unsigned char *)calloc(g_cap, 1);
    for (i = 0; i < oc; i++) {
        if (ou[i]) {
            size_t s;
            a001087_find(ok + i * (size_t)g_W, &s);
            memcpy(g_keys + s * (size_t)g_W, ok + i * (size_t)g_W, sizeof(uint64_t) * (size_t)g_W);
            g_vals[s] = ov[i];
            g_used[s] = 1;
        }
    }
    free(ok);
    free(ov);
    free(ou);
}

static unsigned __int128 a001087_cnt(const uint64_t *C, int d)
{
    int W = g_W, w, i = -1, all;
    uint64_t *A, *B;
    size_t slot;
    unsigned __int128 r;
    for (w = 0; w < W; w++)
        if (C[w]) {
            i = w * 64 + __builtin_ctzll(C[w]);
            break;
        }
    if (i < 0)
        return 1;
    if (a001087_find(C, &slot))
        return g_vals[slot];
    A = g_buf + (size_t)d * 2 * W;
    B = A + W;
    all = 1;
    for (w = 0; w < W; w++) {
        A[w] = C[w];
        B[w] = C[w] & g_nbr[(size_t)i * W + w];
    }
    A[i / 64] &= ~(1ULL << (i % 64));
    for (w = 0; w < W; w++)
        if (A[w] != B[w]) {
            all = 0;
            break;
        }
    if (all)
        r = 2 * a001087_cnt(A, d + 1);
    else
        r = a001087_cnt(A, d + 1) + a001087_cnt(B, d + 1);
    if ((g_cnt + 1) * 2 > g_cap)
        a001087_grow();
    a001087_find(C, &slot);
    memcpy(g_keys + slot * (size_t)W, C, sizeof(uint64_t) * (size_t)W);
    g_vals[slot] = r;
    g_used[slot] = 1;
    g_cnt++;
    return r;
}

seq_t a(idx_t n)
{
    static unsigned __int128 cache[4096];
    static unsigned char have[4096];
    unsigned __int128 v;
    uint64_t N, i, j, hi, lo;
    uint64_t *C;
    int W;
    seq_t res;

    if (n < 1)
        return 0;
    if (n < 4096 && have[n]) {
        v = cache[n];
    } else {
        N = (uint64_t)n;
        W = (int)((N + 63) / 64);
        g_W = W;
        g_nbr = (uint64_t *)calloc((size_t)N * W, sizeof(uint64_t));
        for (i = 1; i < N; i++)
            for (j = 1; j < N; j++) {
                uint64_t x = i & j;
                if (x != 0 && x != i && x != j)
                    g_nbr[i * W + j / 64] |= 1ULL << (j % 64);
            }
        g_buf = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(N + 3) * 2 * W);
        g_cap = 1024;
        g_cnt = 0;
        g_keys = (uint64_t *)malloc(sizeof(uint64_t) * g_cap * (size_t)W);
        g_vals = (unsigned __int128 *)malloc(sizeof(unsigned __int128) * g_cap);
        g_used = (unsigned char *)calloc(g_cap, 1);
        C = (uint64_t *)calloc((size_t)W, sizeof(uint64_t));
        for (i = 1; i < N; i++)
            C[i / 64] |= 1ULL << (i % 64);
        v = a001087_cnt(C, 0);
        free(C);
        free(g_nbr);
        free(g_buf);
        free(g_keys);
        free(g_vals);
        free(g_used);
        if (n < 4096) {
            cache[n] = v;
            have[n] = 1;
        }
    }
    hi = (uint64_t)(v >> 64);
    lo = (uint64_t)v;
    if (hi == 0)
        return (seq_t)lo;
    res = (seq_t)hi;
    res <<= 32;
    res <<= 32;
    res += (seq_t)lo;
    return res;
}
