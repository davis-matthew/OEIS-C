/*
 * A006176  Witt vector *5!.
 * https://oeis.org/A006176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*((5n)!/(n!)^5 - Sum_{d|n, d<n} d*a(d)^(n/d)), i.e.
 *            Sum_{d|n} d*a(d)^(n/d) = (5n)!/(n!)^5 = A008978(n) (ghost
 *            components); a(1) = 120
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

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

/* exact path for fixed-width builds (only n <= 12 can fit in 128 bits) */
static unsigned __int128 fx(idx_t n)
{
    unsigned __int128 b = 1, c, t, s;
    idx_t k, i, d, j;
    for (k = 2; k <= 5; k++) {
        c = 1;
        for (i = 1; i <= n; i++)
            c = c * (unsigned __int128)(k * n - n + i) / i;
        b *= c;
    }
    s = b;
    for (d = 1; d < n; d++) {
        if (n % d != 0) continue;
        c = fx(d);
        t = 1;
        for (j = 0; j < n / d; j++) t *= c;
        s -= (unsigned __int128)d * t;
    }
    return s / n;
}

/* (5n)!/(n!)^5 via Legendre's formula, as a product of prime powers */
static seq_t multinom5(idx_t n)
{
    idx_t m = 5 * n;
    char *comp = (char *)calloc((size_t)m + 1, 1);
    seq_t r = 1;
    uint64_t acc = 1;
    idx_t p, q, pk, e;
    for (p = 2; p <= m; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= m; q += p) comp[q] = 1;
        e = 0;
        for (pk = p; pk <= m; pk *= p) {
            e += m / pk - 5 * (n / pk);
            if (pk > m / p) break;
        }
        while (e > 0) {
            if (acc * p > 0xFFFFFFFFULL) {
                r *= (idx_t)acc;
                acc = 1;
            }
            acc *= p;
            e--;
        }
    }
    r *= (idx_t)acc;
    free(comp);
    return r;
}

static seq_t *memo = NULL;
static idx_t cap = 0;

static seq_t big_a(idx_t n)
{
    idx_t d;
    seq_t s;
    if (n >= cap) {
        idx_t nc = cap ? cap : 16;
        while (nc <= n) nc *= 2;
        memo = grow_zero(memo, cap, nc);
        cap = nc;
    }
    if (memo[n] != 0) return memo[n];
    s = multinom5(n);
    for (d = 1; d < n; d++)
        if (n % d == 0)
            s -= (seq_t)d * ipow_idx(big_a(d), n / d);
    s /= n;
    memo[n] = s;
    return s;
}

seq_t a(idx_t n)
{
    if (seq_is_fixed_width()) {
        if (n > 12) return 0;
        return u128_to_seq(fx(n));
    }
    return big_a(n);
}
