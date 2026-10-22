/*
 * A004867  Numbers that are the sum of at most 5 positive 7th powers.
 * https://oeis.org/A004867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   enumerate all sums of at most 5 positive 7th powers up to a
 *            growing bound, sort, deduplicate; a(n) = n-th smallest (a(1) =
 *            0)
 * Range:     u8 n <= 11, u16 n <= 121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004867_cmp, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a004867_cmp.h"

static uint64_t *a004867_list = NULL;
static size_t a004867_cnt = 0;
static size_t a004867_cap = 0;
static uint64_t a004867_pw[1024];
static uint64_t a004867_L = 0;

static void a004867_push(uint64_t v) {
    if (a004867_cnt == a004867_cap) {
        size_t nc = a004867_cap ? a004867_cap * 2 : 4096;
        a004867_list = (uint64_t *)realloc(a004867_list, nc * sizeof(uint64_t));
        a004867_cap = nc;
    }
    a004867_list[a004867_cnt++] = v;
}

static void a004867_rec(int depth, int maxb, uint64_t sum) {
    int b;
    a004867_push(sum);
    if (depth == 5) return;
    for (b = 1; b <= maxb; b++) {
        if (a004867_pw[b] > a004867_L - sum) break;
        a004867_rec(depth + 1, b, sum + a004867_pw[b]);
    }
}

static void a004867_build(uint64_t L) {
    int nb = 0, b;
    size_t i, k;
    a004867_L = L;
    a004867_cnt = 0;
    for (b = 1; b < 1024; b++) {
        uint64_t p = 1;
        bool over = false;
        int e;
        for (e = 0; e < 7; e++) {
            if (p > L / (uint64_t)b) { over = true; break; }
            p *= (uint64_t)b;
        }
        if (over || p > L) break;
        a004867_pw[b] = p;
        nb = b;
    }
    a004867_rec(0, nb, 0);
    qsort(a004867_list, a004867_cnt, sizeof(uint64_t), a004867_cmp);
    k = 0;
    for (i = 0; i < a004867_cnt; i++) {
        if (k == 0 || a004867_list[k - 1] != a004867_list[i])
            a004867_list[k++] = a004867_list[i];
    }
    a004867_cnt = k;
}

seq_t a(idx_t n) {
    while (a004867_cnt < n) {
        uint64_t L = a004867_L ? a004867_L * 4 : ((uint64_t)1 << 20);
        a004867_build(L);
    }
    return u128_to_seq((unsigned __int128)a004867_list[n - 1]);
}
