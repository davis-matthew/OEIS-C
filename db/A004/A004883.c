/*
 * A004883  Numbers that are the sum of at most 10 nonzero 8th powers.
 * https://oeis.org/A004883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate all sums of at most 10 eighth powers up to a
 *            bound L (multisets, non-increasing order), then sort and remove
 *            duplicates; L is enlarged until at least n terms are found. a(n)
 *            is the n-th smallest such sum
 * Range:     u8 n <= 11, u16 n <= 285, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4883_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a4883_cmp.h"

static uint64_t *a4883_buf = NULL;
static size_t a4883_blen = 0, a4883_bcap = 0;
static uint64_t a4883_pw[256];
static int a4883_npw = 0;

static void a4883_push(uint64_t v) {
    if (a4883_blen == a4883_bcap) {
        a4883_bcap = a4883_bcap ? a4883_bcap * 2 : 4096;
        a4883_buf = (uint64_t *)realloc(a4883_buf, a4883_bcap * sizeof(uint64_t));
    }
    a4883_buf[a4883_blen++] = v;
}

static void a4883_dfs(int maxi, int left, uint64_t sum, uint64_t L) {
    int i;
    a4883_push(sum);
    if (left == 0) return;
    for (i = 0; i <= maxi; i++) {
        if (a4883_pw[i] > L - sum) break;
        a4883_dfs(i, left - 1, sum + a4883_pw[i], L);
    }
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    static size_t cnt = 0;

    if (n < 1) n = 1;
    if (L == 0 || cnt < n) {
        uint64_t newL = L ? L : ((uint64_t)1 << 20);
        for (;;) {
            uint64_t k;
            size_t i, w;
            a4883_npw = 0;
            for (k = 1; k < 200; k++) {
                uint64_t p = k * k;
                p = p * p;
                p = p * p;
                if (p > newL) break;
                a4883_pw[a4883_npw++] = p;
            }
            a4883_blen = 0;
            a4883_dfs(a4883_npw - 1, 10, 0, newL);
            qsort(a4883_buf, a4883_blen, sizeof(uint64_t), a4883_cmp);
            w = 0;
            for (i = 0; i < a4883_blen; i++) {
                if (w == 0 || a4883_buf[i] != a4883_buf[w - 1]) a4883_buf[w++] = a4883_buf[i];
            }
            a4883_blen = w;
            cnt = w;
            L = newL;
            if (cnt >= n || newL > ((uint64_t)1 << 58)) break;
            newL *= 4;
        }
    }
    if (n > cnt) return (seq_t)0;
    return (seq_t)a4883_buf[n - 1];
}
