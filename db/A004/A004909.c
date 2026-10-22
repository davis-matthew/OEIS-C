/*
 * A004909  Numbers that are the sum of at most 3 positive 11th powers.
 * https://oeis.org/A004909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- list every sum x^11 + y^11 + z^11 with 0 <= x <= y <= z
 *            <= K and keep the ones below (K+1)^11, so the list is complete
 *            up to that bound. Sort it, remove duplicates, and increase K
 *            until there are at least n terms
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef unsigned __int128 u128_t;

static u128_t *memo = NULL;
static idx_t memo_len = 0;

static int cmp_u128(const void *pa, const void *pb) {
    u128_t x = *(const u128_t *)pa, y = *(const u128_t *)pb;
    return (x > y) - (x < y);
}

static void build(idx_t K) {
    u128_t *pw = (u128_t *)malloc((size_t)(K + 2) * sizeof(u128_t));
    for (idx_t i = 0; i <= K + 1; i++) {
        u128_t p = 1;
        for (int e = 0; e < 11; e++) p *= (u128_t)i;
        pw[i] = p;
    }
    u128_t bound = pw[K + 1];
    size_t cap = (size_t)((K + 1) * (K + 2) * (K + 3) / 6 + 1);
    u128_t *s = (u128_t *)malloc(cap * sizeof(u128_t));
    size_t cnt = 0;
    for (idx_t i = 0; i <= K; i++)
        for (idx_t j = i; j <= K; j++) {
            u128_t t = pw[i] + pw[j];
            if (t >= bound) break;
            for (idx_t k = j; k <= K; k++) {
                u128_t v = t + pw[k];
                if (v >= bound) break;
                s[cnt++] = v;
            }
        }
    qsort(s, cnt, sizeof(u128_t), cmp_u128);
    size_t u = 0;
    for (size_t i = 0; i < cnt; i++)
        if (u == 0 || s[i] != s[u - 1]) s[u++] = s[i];
    free(pw);
    free(memo);
    memo = s;
    memo_len = (idx_t)u;
}

static seq_t to_seq(u128_t v) {
    uint64_t hi = (uint64_t)(v >> 64), lo = (uint64_t)v;
    seq_t r = 0;
    if (hi) {
        r = (seq_t)hi;
        for (int i = 0; i < 8; i++) r = (seq_t)(r * (seq_t)256);
    }
    r += (seq_t)lo;
    return r;
}

seq_t a(idx_t n) {
    idx_t K = 16;
    while (memo_len < n) {
        build(K);
        K *= 2;
    }
    return to_seq(memo[n - 1]);
}
