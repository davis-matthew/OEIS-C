/*
 * A054945  Number of strongly connected labeled digraphs on n nodes with an
 *          odd number of edges.
 * https://oeis.org/A054945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A003030(n) - (n-1)!)/2, where A003030 is computed by
 *            Robinson's recurrence: c(m) = 2^(m(m-1)) - Sum_{k=1..m-1}
 *            binomial(m,k)*c(k)*2^(k(m-k))*2^((m-k)(m-k-1)), c(1)=1;
 *            A003030(m) = c(m) + Sum_{k=1..m-1}
 *            binomial(m-1,k-1)*A003030(k)*c(m-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A054945_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    idx_t i;
    for (i = 0; i < newlen; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static seq_t a3030_two_pow(idx_t e) {
    seq_t r = 1;
    seq_t b = 2;
    while (e) {
        if (e & 1) r = r * b;
        e >>= 1;
        if (e) b = b * b;
    }
    return r;
}

static seq_t *a3030_c = NULL;
static seq_t *a3030_s = NULL;
static seq_t *a3030_row = NULL;
static idx_t a3030_done = 0;
static idx_t a3030_cap = 0;

static void a3030_ensure(idx_t n) {
    if (n + 2 <= a3030_cap) return;
    idx_t nc = a3030_cap ? a3030_cap : 16;
    while (nc < n + 2) nc *= 2;
    a3030_c = A054945_grow(a3030_c, a3030_cap, nc);
    a3030_s = A054945_grow(a3030_s, a3030_cap, nc);
    a3030_row = A054945_grow(a3030_row, a3030_cap, nc);
    if (a3030_cap == 0) {
        a3030_row[0] = 1;
        a3030_c[0] = 0;
        a3030_s[0] = 0;
    }
    a3030_cap = nc;
}

static seq_t A003030_a(idx_t n) {
    if (n == 0) return 0;
    a3030_ensure(n);
    while (a3030_done < n) {
        idx_t m = a3030_done + 1;
        idx_t k;
        seq_t sum_s = 0;
        for (k = 1; k < m; k++)
            sum_s += a3030_row[k - 1] * a3030_s[k] * a3030_c[m - k];
        a3030_row[m] = 1;
        for (k = m - 1; k >= 1; k--)
            a3030_row[k] += a3030_row[k - 1];
        seq_t sub = 0;
        for (k = 1; k < m; k++) {
            idx_t r = m - k;
            sub += a3030_row[k] * a3030_c[k] * a3030_two_pow(k * r + r * (r - 1));
        }
        seq_t cm = a3030_two_pow(m * (m - 1)) - sub;
        a3030_c[m] = cm;
        a3030_s[m] = cm + sum_s;
        a3030_done = m;
    }
    return a3030_s[n];
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t f = 1;
    idx_t i;
    for (i = 2; i < n; i++) f = f * (seq_t)i;
    seq_t t = A003030_a(n);
    return (t - f) / 2;
}
