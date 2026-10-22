/*
 * A007441  1 + Sum_{n>=1} a_n x^n = Product_{n>=1} (1-x^n)^prime(n).
 * https://oeis.org/A007441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n)*Sum_{k=1..n} a(n-k)*b(k), a(0) = 1, b(k) =
 *            -Sum_{d|k} d*prime(d)
 * Range:     i8 n <= 13, i16 n <= 33, i32 n <= 76, i64 n <= 177,
 *            i128 n <= 445, big any n
 * Helpers:   from_i128, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/grow_zero.h"
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"

static int64_t *pr = NULL;   /* pr[1..pr_n] = primes */
static idx_t pr_n = 0;

static void ensure_primes(idx_t n) {
    if (pr_n >= n) return;
    idx_t lim = 15 * n + 30;
    for (;;) {
        bool *comp = (bool *)calloc(lim + 1, sizeof(bool));
        idx_t cnt = 0;
        for (idx_t i = 2; i <= lim; i++) {
            if (!comp[i]) {
                cnt++;
                if (i <= lim / i)
                    for (idx_t j = i * i; j <= lim; j += i) comp[j] = true;
            }
        }
        if (cnt >= n) {
            free(pr);
            pr = (int64_t *)malloc((cnt + 1) * sizeof(int64_t));
            idx_t c = 0;
            for (idx_t i = 2; i <= lim; i++)
                if (!comp[i]) pr[++c] = (int64_t)i;
            pr_n = cnt;
            free(comp);
            return;
        }
        free(comp);
        lim *= 2;
    }
}

static int64_t *bb = NULL;   /* bb[1..bb_n] = b(k) */
static idx_t bb_n = 0;

static void ensure_b(idx_t n) {
    if (bb_n >= n) return;
    idx_t m = n < 64 ? 64 : n * 2;
    ensure_primes(m);
    free(bb);
    bb = (int64_t *)calloc(m + 1, sizeof(int64_t));
    for (idx_t d = 1; d <= m; d++) {
        int64_t v = (int64_t)d * pr[d];
        for (idx_t k = d; k <= m; k += d) bb[k] -= v;
    }
    bb_n = m;
}

/* fixed-width path: exact intermediates in __int128 */
static __int128 *m128 = NULL;
static idx_t m128_cap = 0, m128_done = 0;

static seq_t a_fixed(idx_t n) {
    if (n + 1 > m128_cap) {
        idx_t nc = n + 1 < 64 ? 64 : 2 * (n + 1);
        m128 = (__int128 *)realloc(m128, nc * sizeof(__int128));
        m128_cap = nc;
    }
    if (m128_done == 0) { m128[0] = 1; m128_done = 1; }
    ensure_b(n);
    for (idx_t i = m128_done; i <= n; i++) {
        __int128 s = 0;
        for (idx_t k = 1; k <= i; k++) s += m128[i - k] * (__int128)bb[k];
        m128[i] = s / (__int128)i;
    }
    if (n + 1 > m128_done) m128_done = n + 1;
    return from_i128(m128[n]);
}

/* arbitrary-precision path */
static seq_t *mb = NULL;
static idx_t mb_cap = 0, mb_done = 0;

static seq_t a_big(idx_t n) {
    if (n + 1 > mb_cap) {
        idx_t nc = n + 1 < 64 ? 64 : 2 * (n + 1);
        mb = grow_zero(mb, mb_cap, nc);
        mb_cap = nc;
    }
    if (mb_done == 0) { mb[0] = 1; mb_done = 1; }
    ensure_b(n);
    for (idx_t i = mb_done; i <= n; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= i; k++) s += mb[i - k] * (seq_t)bb[k];
        mb[i] = s / (seq_t)i;
    }
    if (n + 1 > mb_done) mb_done = n + 1;
    return mb[n];
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed(n);
    return a_big(n);
}
