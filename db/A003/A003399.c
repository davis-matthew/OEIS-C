/*
 * A003399  Sum of 10 positive 9th powers.
 * https://oeis.org/A003399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate every multiset of 10 positive 9th powers
 *            whose sum is <= L, sort and deduplicate the sums, and double L
 *            until there are at least n distinct sums; a(n) is the n-th
 *            smallest
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3399_cmp, a3399_push
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3399_cmp.h"
#include "../utils/a3399_push.h"

static uint64_t a3399_pw[128];
static int a3399_pw_ready = 0;

static void a3399_rec(int k, int maxb, uint64_t sum, uint64_t L,
                      uint64_t **arr, size_t *cnt, size_t *cap) {
    int b;
    if (k == 0) {
        a3399_push(arr, cnt, cap, sum);
        return;
    }
    for (b = 1; b <= maxb; b++) {
        uint64_t p = a3399_pw[b];
        if (sum + p + (uint64_t)(k - 1) > L) break;
        a3399_rec(k - 1, b, sum + p, L, arr, cnt, cap);
    }
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static size_t cache_len = 0;
    static uint64_t L = 0;

    if (!a3399_pw_ready) {
        int b, j;
        for (b = 0; b < 128; b++) {
            uint64_t p = 1;
            for (j = 0; j < 9; j++) p *= (uint64_t)b;
            a3399_pw[b] = p;
        }
        a3399_pw_ready = 1;
    }

    while ((idx_t)cache_len < n) {
        uint64_t *arr = NULL;
        size_t cnt = 0, cap = 0, u, i;
        L = L ? L * 2 : ((uint64_t)1 << 16);
        if (L > (uint64_t)1000000000000000000ULL) L = (uint64_t)1000000000000000000ULL;
        a3399_rec(10, 127, 0, L, &arr, &cnt, &cap);
        if (cnt > 0) qsort(arr, cnt, sizeof(uint64_t), a3399_cmp);
        u = 0;
        for (i = 0; i < cnt; i++)
            if (u == 0 || arr[i] != arr[u - 1]) arr[u++] = arr[i];
        free(cache);
        cache = arr;
        cache_len = u;
        if (L >= (uint64_t)1000000000000000000ULL) break;
    }
    if (n < 1 || (idx_t)cache_len < n) return (seq_t)0;
    return (seq_t)cache[n - 1];
}
