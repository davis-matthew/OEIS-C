/*
 * A004806  Numbers that are the sum of 6 positive 10th powers.
 * https://oeis.org/A004806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search. Enumerate all sums of 6 positive 10th powers with bases
 *            <= M and keep those below (M+1)^10 + 5 (that range is complete).
 *            Sort and dedupe. If there are fewer than n terms, increase M and
 *            repeat
 * Range:     u8 none, u16 n <= 7, u32 n <= 209, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_array, cmp_seq, grow_arr, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_arr.h"
#include "../utils/alloc_array.h"
#include "../utils/release.h"
#include "../utils/cmp_seq.h"

static seq_t pw[256];
static seq_t lim;
static int maxb;
static seq_t *buf = NULL;
static idx_t cnt = 0, cap = 0;

static void push(seq_t v) {
    if (cnt == cap) {
        idx_t ncap = cap ? cap * 2 : 1024;
        buf = grow_arr(buf, cap, ncap);
        cap = ncap;
    }
    buf[cnt++] = v;
}

static void rec(int k, int start, seq_t s) {
    if (k == 0) {
        push(s);
        return;
    }
    for (int b = start; b <= maxb; b++) {
        if (s + pw[b] * k >= lim) break;
        rec(k - 1, b, s + pw[b]);
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static int cur_m = 4;

    while (cache == NULL || cache_len < n) {
        if (cache != NULL) {
            release(cache, cache_len);
            cache = NULL;
            cur_m += 4;
        }
        maxb = cur_m;
        for (int b = 1; b <= maxb + 1; b++) {
            seq_t p = 1;
            for (int i = 0; i < 10; i++) p *= b;
            pw[b] = p;
        }
        lim = pw[maxb + 1] + 5;
        buf = NULL;
        cnt = 0;
        cap = 0;
        rec(6, 1, (seq_t)0);

        qsort(buf, cnt, sizeof(seq_t), cmp_seq);
        idx_t u = 0;
        for (idx_t i = 0; i < cnt; i++) {
            if (u == 0 || buf[i] != buf[u - 1]) {
                if (u != i) buf[u] = buf[i];
                u++;
            }
        }
        cache = buf;
        cache_len = u;
        if (u == 0) {
            cache = alloc_array(1);
            cache_len = 0;
        }
        buf = NULL;
        cnt = cap = 0;
    }
    return cache[n - 1];
}
