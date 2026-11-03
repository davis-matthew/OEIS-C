/*
 * A005380  Expansion of 1 / Product_{k>=1} (1-x^k)^(k+1).
 * https://oeis.org/A005380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} (sigma_1(k) + sigma_2(k)) * a(n-k),
 *            a(0) = 1 (Euler transform of b(n) = n+1)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 39, u64 n <= 105, u128 n <= 288,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 9620 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static uint64_t sigma_sum(idx_t k) {
    uint64_t s1 = 0, s2 = 0;
    idx_t d;
    for (d = 1; d * d <= k; d++) {
        if (k % d == 0) {
            idx_t e = k / d;
            s1 += d;
            s2 += (uint64_t)d * d;
            if (e != d) {
                s1 += e;
                s2 += (uint64_t)e * e;
            }
        }
    }
    return s1 + s2;
}

static uint64_t *sig_cache = NULL;
static idx_t sig_len = 0;

static unsigned __int128 *fmemo = NULL;
static idx_t fmemo_len = 0;

static seq_t *bmemo = NULL;
static idx_t bmemo_len = 0;

static void ensure_sigma(idx_t n) {
    idx_t k;
    if (sig_len > n) return;
    sig_cache = (uint64_t *)realloc(sig_cache, (size_t)(n + 1) * sizeof(uint64_t));
    for (k = sig_len; k <= n; k++) sig_cache[k] = (k == 0) ? 0 : sigma_sum(k);
    sig_len = n + 1;
}

seq_t a(idx_t n) {
    idx_t i, k;
    ensure_sigma(n);
    if (seq_is_fixed_width()) {
        /* exact for every value that fits in 64 bits: n*a(n) stays below 2^128 */
        if (fmemo_len <= n) {
            fmemo = (unsigned __int128 *)realloc(fmemo, (size_t)(n + 1) * sizeof(unsigned __int128));
            if (fmemo_len == 0) {
                fmemo[0] = 1;
                fmemo_len = 1;
            }
            for (i = fmemo_len; i <= n; i++) {
                unsigned __int128 s = 0;
                for (k = 1; k <= i; k++)
                    s += (unsigned __int128)sig_cache[k] * fmemo[i - k];
                fmemo[i] = s / (unsigned __int128)i;
            }
            fmemo_len = n + 1;
        }
        return u128_to_seq(fmemo[n]);
    } else {
        if (bmemo_len <= n) {
            idx_t old = bmemo_len;
            if (bmemo == NULL) {
                bmemo = alloc_zero(n + 1);
                bmemo[0] = 1;
                old = 1;
            } else {
                bmemo = grow_zero(bmemo, bmemo_len, n + 1);
            }
            bmemo_len = n + 1;
            for (i = old; i <= n; i++) {
                seq_t s = 0;
                for (k = 1; k <= i; k++)
                    s += bmemo[i - k] * (seq_t)sig_cache[k];
                bmemo[i] = s / (seq_t)i;
            }
        }
        return bmemo[n];
    }
}
