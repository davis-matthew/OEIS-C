/*
 * A004910  Numbers that are the sum of at most 4 positive 11th powers.
 * https://oeis.org/A004910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- list every sum of at most 4 positive 11th powers below
 *            a growing bound B^11, sort, dedupe, take the n-th
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width, sift_down
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/sift_down.h"

static void heap_sort(seq_t *v, idx_t len) {
    if (len < 2) return;
    idx_t i = len / 2;
    while (i > 0) { i--; sift_down(v, i, len); }
    idx_t end = len;
    while (end > 1) {
        end--;
        seq_t t = v[0]; v[0] = v[end]; v[end] = t;
        sift_down(v, 0, end);
    }
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_alloc = 0;
static bool cache_sat = false;

static seq_t g_limit, g_max;
static bool g_sat;

static bool fits(seq_t s, seq_t p) {
    if (g_sat) return s <= g_max - p;
    return s < g_limit - p;
}

static void build(idx_t B) {
    bool fixed = seq_is_fixed_width();
    seq_t mx = 0;
    if (fixed) mx = mx - 1;
    g_max = mx;

    seq_t *pw = alloc_zero(B + 1);
    pw[0] = 0;
    idx_t nb = 0;
    bool sat = false;
    for (idx_t b = 1; b <= B; b++) {
        seq_t p = 1;
        bool ovf = false;
        for (int e = 0; e < 11; e++) {
            if (fixed && p > mx / (seq_t)b) { ovf = true; break; }
            p *= (seq_t)b;
        }
        if (ovf) { sat = true; break; }
        pw[b] = p;
        nb = b;
    }
    idx_t m;
    g_sat = sat;
    if (sat) {
        m = nb + 1;
        g_limit = mx;
    } else {
        m = B;
        g_limit = pw[B];
    }

    idx_t bound = (m + 3) * (m + 2) / 2 * (m + 1) / 3 * m / 4 + 1;
    seq_t *v = alloc_zero(bound);
    idx_t cnt = 0;
    for (idx_t i = 0; i < m; i++) {
        seq_t s1 = pw[i];
        for (idx_t j = i; j < m; j++) {
            if (!fits(s1, pw[j])) break;
            seq_t s2 = s1 + pw[j];
            for (idx_t k = j; k < m; k++) {
                if (!fits(s2, pw[k])) break;
                seq_t s3 = s2 + pw[k];
                for (idx_t l = k; l < m; l++) {
                    if (!fits(s3, pw[l])) break;
                    v[cnt++] = s3 + pw[l];
                }
            }
        }
    }
    release(pw, B + 1);

    heap_sort(v, cnt);
    idx_t u = 0;
    for (idx_t i = 0; i < cnt; i++) {
        if (u == 0 || v[i] != v[u - 1]) v[u++] = v[i];
    }

    if (cache) release(cache, cache_alloc);
    cache = v;
    cache_alloc = bound;
    cache_len = u;
    cache_sat = sat;
}

seq_t a(idx_t n) {
    idx_t B = 16;
    while (n > cache_len && !cache_sat) {
        build(B);
        B *= 2;
    }
    if (n <= cache_len) return cache[n - 1];
    return cache[cache_len - 1];
}
