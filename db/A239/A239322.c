/*
 * A239322  Interleave (-1)^n*(A000182(n+1) - A000364(n)), -A028296(n+1).
 * https://oeis.org/A239322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(2m) = (-1)^(m+1)*(A000182(m+1) - A000364(m)), a(2m+1) =
 *            -A028296(m+1)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

#define A182_MAXN 10000
#define A182_ROWS (2 * A182_MAXN + 2)

static seq_t a182_bufA[A182_ROWS];
static seq_t a182_bufB[A182_ROWS];
static seq_t a182_res[A182_MAXN + 1];
static seq_t *a182_prev = a182_bufA;
static seq_t *a182_cur = a182_bufB;
static idx_t a182_m = 0;
static idx_t a182_done = 0;
static bool a182_init = false;

static seq_t A000182_a(idx_t n)
{
    if (!a182_init) {
        a182_prev[0] = 1;
        a182_init = true;
    }
    if (n < 1 || n > A182_MAXN)
        return 0;
    while (a182_done < n) {
        idx_t m = ++a182_m;
        idx_t k;
        seq_t *t;
        a182_cur[0] = 0;
        for (k = 1; k <= m; k++)
            a182_cur[k] = a182_cur[k - 1] + a182_prev[m - k];
        t = a182_prev;
        a182_prev = a182_cur;
        a182_cur = t;
        if (m & 1) {
            a182_done = (m + 1) / 2;
            a182_res[a182_done] = a182_prev[m];
        }
    }
    return a182_res[n];
}

static seq_t *a364_vals = NULL;
static idx_t a364_have = 0;

static seq_t A000364_a(idx_t n) {
    if (n < a364_have)
        return a364_vals[n];

    idx_t N = n;
    if (N < 2 * a364_have) N = 2 * a364_have;
    if (N < 16) N = 16;
    idx_t M = 2 * N;

    seq_t *p = mem_new(seq_t, (size_t)(M + 2));
    seq_t *q = mem_new(seq_t, (size_t)(M + 2));
    seq_t *R = mem_new(seq_t, (size_t)(N + 1));

    p[0] = 1;
    R[0] = 1;
    for (idx_t m = 1; m <= M; m++) {
        q[0] = 0;
        for (idx_t k = 1; k <= m; k++)
            q[k] = q[k - 1] + p[m - k];
        seq_t *t = p; p = q; q = t;
        if ((m & 1) == 0)
            R[m / 2] = p[m];
    }

    mem_free(p);
    mem_free(q);
    if (a364_vals != NULL) mem_free(a364_vals);
    a364_vals = R;
    a364_have = N + 1;
    return a364_vals[n];
}

static seq_t *g_A = NULL;
static size_t g_A_cap = 0;
static idx_t g_cur_max = 0;

static seq_t A028296_a(idx_t n)
{
    if (g_A == NULL) {
        g_A_cap = 8;
        g_A = mem_new(seq_t, g_A_cap);
        for (size_t i = 0; i < g_A_cap; ++i) g_A[i] = 0;
        g_A[0] = 1;
        g_cur_max = 0;
    }

    if ((size_t)n >= g_A_cap) {
        size_t new_cap = g_A_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, g_A, new_cap);
        if (tmp == NULL) {
            return 0;
        }
        g_A = tmp;
        g_A_cap = new_cap;
    }

    for (idx_t k = g_cur_max + 1; k <= n; ++k) {
        seq_t sum = 0;
        seq_t binom = 1;
        for (idx_t i = 0; i < k; ++i) {
            sum += g_A[i] * binom;
            if (i < k - 1) {
                seq_t aik = ((seq_t)k)*2 - ((seq_t)i)*2;
                seq_t num = aik * (aik - 1);
                seq_t den = ((seq_t)i*2 + 2) * ((seq_t)i*2 + 1);
                binom = (binom * num) / den;
            }
        }
        g_A[k] = -sum;
    }
    if (n > g_cur_max) g_cur_max = n;
    return g_A[n];
}

seq_t a(idx_t n)
{
    idx_t m = n / 2;
    if (n & 1) {
        return -A028296_a(m + 1);
    }
    seq_t v = A000182_a(m + 1) - A000364_a(m);
    if ((m & 1) == 0)
        return -v;
    return v;
}
