/*
 * A004812  Numbers that are the sum of 12 positive 10th powers.
 * https://oeis.org/A004812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: enumerate every sum of 12 positive 10th powers up to a
 *            bound (doubling the bound as needed), then sort and remove
 *            duplicates; a(n) is the n-th smallest
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4812_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a4812_cmp.h"

static uint64_t a4812_pw[90];
static int a4812_npw;
static uint64_t *a4812_buf = NULL;
static size_t a4812_cnt = 0, a4812_cap = 0;

static void a4812_push(uint64_t v) {
    if (a4812_cnt == a4812_cap) {
        a4812_cap = a4812_cap ? a4812_cap * 2 : 4096;
        a4812_buf = (uint64_t *)realloc(a4812_buf, a4812_cap * sizeof(uint64_t));
    }
    a4812_buf[a4812_cnt++] = v;
}

static void a4812_gen(uint64_t cur, int rem, int minb, uint64_t lim) {
    if (rem == 0) {
        a4812_push(cur);
        return;
    }
    for (int b = minb; b < a4812_npw; b++) {
        uint64_t p = a4812_pw[b];
        if (cur + (uint64_t)rem * p > lim) break;
        a4812_gen(cur + p, rem - 1, b, lim);
    }
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static idx_t clen = 0;
    static uint64_t lim = (uint64_t)1 << 20;

    while (cache == NULL || n > clen) {
        /* powers b^10 for b = 1.. while b^10 <= lim */
        a4812_npw = 1;
        a4812_pw[0] = 0;
        for (int b = 1; b <= 84; b++) {
            uint64_t p = 1;
            for (int i = 0; i < 10; i++) p *= (uint64_t)b;
            if (p > lim) break;
            a4812_pw[b] = p;
            a4812_npw = b + 1;
        }
        a4812_cnt = 0;
        a4812_gen(0, 12, 1, lim);
        if (a4812_cnt > 0) {
            qsort(a4812_buf, a4812_cnt, sizeof(uint64_t), a4812_cmp);
            size_t j = 1;
            for (size_t i = 1; i < a4812_cnt; i++)
                if (a4812_buf[i] != a4812_buf[j - 1]) a4812_buf[j++] = a4812_buf[i];
            free(cache);
            cache = (uint64_t *)malloc(j * sizeof(uint64_t));
            for (size_t i = 0; i < j; i++) cache[i] = a4812_buf[i];
            clen = (idx_t)j;
        }
        if (n <= clen && cache != NULL) break;
        if (lim > ((uint64_t)1 << 59)) break;
        lim *= 2;
    }
    if (cache == NULL || n > clen) return 0;
    return (seq_t)cache[n - 1];
}
