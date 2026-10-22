/*
 * A004800  Sum of 11 positive 9th powers.
 * https://oeis.org/A004800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate every nondecreasing 11-tuple of positive 9th
 *            powers with sum <= L, then sort and drop duplicates; double L
 *            until at least n distinct sums are found
 * Range:     u8 n <= 1, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero, heap_sort, pow9_ok, release, seq_is_fixed_width,
 *            sift
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/heap_sort.h"
#include "../utils/pow9_ok.h"
#include "../utils/sift.h"

static seq_t *cache = 0;
static idx_t cache_alloc = 0;
static idx_t cache_len = 0;
static seq_t L = 0;
static bool at_max = false;

static seq_t *pw = 0;
static idx_t npw = 0;
static seq_t *buf = 0;
static idx_t cnt = 0;

static void rec(int k, idx_t i, seq_t s, bool store) {
    seq_t rem, lim;
    idx_t j;
    if (k == 0) {
        if (store) buf[cnt] = s;
        cnt++;
        return;
    }
    rem = L - s;
    lim = rem / k;
    for (j = i; j <= npw && pw[j] <= lim; j++)
        rec(k - 1, j, s + pw[j], store);
}

static void build(void) {
    idx_t b, total, uniq, i;
    seq_t p;

    npw = 0;
    for (b = 1;; b++) {
        if (!pow9_ok(b, &p) || p > L) break;
        npw = b;
    }
    pw = alloc_zero(npw + 1);
    for (b = 1; b <= npw; b++) pow9_ok(b, &pw[b]);

    cnt = 0;
    rec(11, 1, 0, false);
    total = cnt;

    buf = alloc_zero(total + 1);
    cnt = 0;
    rec(11, 1, 0, true);

    heap_sort(buf, total);
    uniq = 0;
    for (i = 0; i < total; i++) {
        if (uniq == 0 || buf[i] != buf[uniq - 1]) {
            buf[uniq] = buf[i];
            uniq++;
        }
    }

    release(pw, npw + 1);
    pw = 0;
    if (cache) release(cache, cache_alloc);
    cache = buf;
    cache_alloc = total + 1;
    cache_len = uniq;
    buf = 0;
}

seq_t a(idx_t n) {
    if (n >= 1 && n <= cache_len) return cache[n - 1];
    if (L == 0) L = 16;
    for (;;) {
        build();
        if (cache_len >= n) return cache[n - 1];
        if (at_max) return 0;
        if (seq_is_fixed_width()) {
            seq_t mx = (seq_t)0 - 1;
            if (L > mx / 2) {
                L = mx;
                at_max = true;
            } else {
                L *= 2;
            }
        } else {
            L *= 2;
        }
    }
}
