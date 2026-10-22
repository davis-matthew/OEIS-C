/*
 * A004816  Numbers that are the sum of 5 positive 11th powers.
 * https://oeis.org/A004816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: enumerate all sums of 5 positive 11th powers
 *            b1<=...<=b5<=B below (B+1)^11+4, sort, dedupe; grow B as needed
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 413, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *cache = NULL;
static idx_t cache_len = 0;
static idx_t cur_B = 0;

static u128 *buf = NULL;
static idx_t buf_len = 0, buf_cap = 0;

static int cmp_u128(const void *a, const void *b) {
    u128 x = *(const u128 *)a;
    u128 y = *(const u128 *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static u128 pow11(idx_t b) {
    u128 p = 1;
    int i;
    for (i = 0; i < 11; i++) p *= (u128)b;
    return p;
}

static void push(u128 v) {
    if (buf_len == buf_cap) {
        idx_t nc = buf_cap ? buf_cap * 2 : 1024;
        u128 *nb = (u128 *)realloc(buf, sizeof(u128) * nc);
        buf = nb;
        buf_cap = nc;
    }
    buf[buf_len++] = v;
}

/* all terms below (B+1)^11 + 4 are complete when bases are limited to B */
static void build(idx_t B) {
    u128 *pw = (u128 *)malloc(sizeof(u128) * (B + 1));
    u128 limit = pow11(B + 1) + 4;
    idx_t i, j, k, l, m;

    for (i = 1; i <= B; i++) pw[i] = pow11(i);

    buf_len = 0;
    for (i = 1; i <= B; i++) {
        u128 s1 = pw[i];
        if (s1 + 4 >= limit) break;
        for (j = i; j <= B; j++) {
            u128 s2 = s1 + pw[j];
            if (s2 + 3 >= limit) break;
            for (k = j; k <= B; k++) {
                u128 s3 = s2 + pw[k];
                if (s3 + 2 >= limit) break;
                for (l = k; l <= B; l++) {
                    u128 s4 = s3 + pw[l];
                    if (s4 + 1 >= limit) break;
                    for (m = l; m <= B; m++) {
                        u128 s5 = s4 + pw[m];
                        if (s5 >= limit) break;
                        push(s5);
                    }
                }
            }
        }
    }
    free(pw);

    if (buf_len > 0) qsort(buf, buf_len, sizeof(u128), cmp_u128);

    free(cache);
    cache = (u128 *)malloc(sizeof(u128) * (buf_len + 1));
    cache_len = 0;
    for (i = 0; i < buf_len; i++) {
        if (i == 0 || buf[i] != buf[i - 1]) cache[cache_len++] = buf[i];
    }
    cur_B = B;
}

seq_t a(idx_t n) {
    if (cur_B == 0) build(8);
    while (cache_len < n && cur_B < 2000) {
        idx_t nb = cur_B + cur_B / 2 + 1;
        if (nb > 2000) nb = 2000;
        build(nb);
    }
    if (n == 0 || n > cache_len) return (seq_t)0;
    return u128_to_seq(cache[n - 1]);
}
