/*
 * A005758  Number of partitions of n into parts of 12 kinds.
 * https://oeis.org/A005758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1, a(n) = (12/n)*Sum_{k=1..n} A000203(k)*a(n-k) for n > 0
 *            (equivalently G.f.: Product_{k>=1} (1 - x^k)^(-12); fixed-width
 *            builds expand that product using additions only, so the results
 *            stay exact modulo 2^w)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 49, u128 n <= 154,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 997 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
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

static seq_t *A_tab = NULL;
static idx_t A_len = 0;
static unsigned long long *sig_tab = NULL;
static idx_t sig_len = 0;

static void ext_sigma(idx_t len) {
    idx_t m, i;
    if (len <= sig_len) return;
    sig_tab = (unsigned long long *)realloc(sig_tab, len * sizeof(unsigned long long));
    for (m = sig_len; m < len; m++) sig_tab[m] = 0;
    for (i = 1; i < len; i++) {
        idx_t start = (sig_len + i - 1) / i * i;
        if (start < i) start = i;
        for (m = start; m < len; m += i) sig_tab[m] += i;
    }
    sig_len = len;
}

seq_t a(idx_t n) {
    idx_t newlen, i, k, m;
    int r;
    if (n < A_len) return A_tab[n];
    newlen = n + 1;
    if (newlen < 2 * A_len) newlen = 2 * A_len;
    A_tab = grow_zero(A_tab, A_len, newlen);
    if (seq_is_fixed_width()) {
        for (i = 0; i < newlen; i++) A_tab[i] = 0;
        A_tab[0] = 1;
        for (k = 1; k < newlen; k++)
            for (r = 0; r < 12; r++)
                for (i = k; i < newlen; i++)
                    A_tab[i] += A_tab[i - k];
    } else {
        ext_sigma(newlen);
        if (A_len == 0) A_tab[0] = 1;
        for (m = (A_len == 0 ? 1 : A_len); m < newlen; m++) {
            seq_t sum = 0;
            for (k = 1; k <= m; k++)
                sum += (seq_t)sig_tab[k] * A_tab[m - k];
            A_tab[m] = sum * 12 / (seq_t)m;
        }
    }
    A_len = newlen;
    return A_tab[n];
}
