/*
 * A004894  Numbers that are the sum of at most 10 positive 9th powers.
 * https://oeis.org/A004894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate every sum of at most 10 ninth powers with
 *            bases <= B, sort and remove duplicates, and keep the sums below
 *            (B+1)^9 (that list is complete). Increase B until there are at
 *            least n terms
 * Range:     u8 n <= 11, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t *cache = NULL;
static idx_t cache_len = 0;

static uint64_t *buf = NULL;
static size_t buf_len = 0, buf_cap = 0;

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void push(uint64_t v) {
    if (buf_len == buf_cap) {
        buf_cap = buf_cap ? buf_cap * 2 : 1024;
        buf = (uint64_t *)realloc(buf, buf_cap * sizeof(uint64_t));
    }
    buf[buf_len++] = v;
}

static void gen(const uint64_t *pw, int minb, int maxb, int left, uint64_t sum) {
    push(sum);
    if (left == 0) return;
    for (int b = minb; b <= maxb; b++)
        gen(pw, b, maxb, left - 1, sum + pw[b]);
}

seq_t a(idx_t n) {
    if (n <= cache_len) return u128_to_seq((unsigned __int128)cache[n - 1]);
    for (int B = 1; ; B++) {
        uint64_t pw[64];
        for (int b = 0; b <= B + 1; b++) {
            uint64_t p = 1;
            for (int k = 0; k < 9; k++) p *= (uint64_t)b;
            pw[b] = p;
        }
        buf_len = 0;
        gen(pw, 1, B, 10, 0);
        qsort(buf, buf_len, sizeof(uint64_t), cmp_u64);
        uint64_t limit = pw[B + 1];
        idx_t cnt = 0;
        for (size_t i = 0; i < buf_len; i++) {
            if (buf[i] >= limit) break;
            if (cnt == 0 || buf[i] != buf[cnt - 1]) buf[cnt++] = buf[i];
        }
        if (cnt >= n) {
            free(cache);
            cache = (uint64_t *)malloc(cnt * sizeof(uint64_t));
            for (idx_t i = 0; i < cnt; i++) cache[i] = buf[i];
            cache_len = cnt;
            return u128_to_seq((unsigned __int128)cache[n - 1]);
        }
    }
}
