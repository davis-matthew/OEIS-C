/*
 * A007301  From expansion of exp(sin x).
 * https://oeis.org/A007301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A002017(2n), where A002017(m) = m! [x^m] exp(sin x)
 *            satisfies (from f' = cos(x) f) A002017(0) = 1, A002017(i+1) =
 *            Sum_{k=0..i, k even} (-1)^(k/2) * binomial(i,k) * A002017(i-k)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

static seq_t *g_vals = 0;   /* g_vals[j] = A002017(j) for j < g_len */
static seq_t *g_row = 0;    /* binomial(g_len-1, k), Pascal row */
static idx_t g_len = 0;
static idx_t g_cap = 0;

seq_t a(idx_t n) {
    idx_t m = 2 * n;

    if (m + 2 > g_cap) {
        idx_t newcap = g_cap * 2;
        if (newcap < m + 2) newcap = m + 2;
        if (newcap < 16) newcap = 16;
        g_vals = grow_zero(g_vals, g_cap, newcap);
        g_row = grow_zero(g_row, g_cap, newcap);
        g_cap = newcap;
    }

    if (g_len == 0) {
        g_vals[0] = 1;
        g_row[0] = 1;
        g_len = 1;
    }

    while (g_len <= m) {
        idx_t i = g_len - 1;
        idx_t k;
        seq_t sum = 0;
        for (k = 0; k <= i; k += 2) {
            if ((k >> 1) & 1)
                sum -= g_row[k] * g_vals[i - k];
            else
                sum += g_row[k] * g_vals[i - k];
        }
        g_vals[i + 1] = sum;
        /* advance Pascal row from i to i+1 (g_row[i+1] is currently 0) */
        for (k = i + 1; k >= 1; k--)
            g_row[k] += g_row[k - 1];
        g_len++;
    }

    return g_vals[m];
}
