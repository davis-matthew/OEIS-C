/*
 * A006216  Number of down-up permutations of n+4 starting with 4.
 * https://oeis.org/A006216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*E(n+2) - E(n), where E(j) = A000111(j) are the Euler
 *            (up/down) numbers, computed with the Entringer/boustrophedon
 *            triangle T(0,0) = 1, T(m,0) = 0, T(m,k) = T(m,k-1) + T(m-1,m-k),
 *            E(m) = T(m,m)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *E_tab = NULL;
static seq_t *prev_row = NULL;
static seq_t *curr_row = NULL;
static idx_t cap = 0;
static idx_t done = 0;
static bool inited = false;

static void ensure(idx_t m) {
    if (!inited) {
        idx_t c = m + 1 < 16 ? 16 : m + 1;
        E_tab = alloc_zero(c);
        prev_row = alloc_zero(c);
        curr_row = alloc_zero(c);
        cap = c;
        E_tab[0] = 1;
        prev_row[0] = 1;
        done = 0;
        inited = true;
    }
    if (m + 1 > cap) {
        idx_t c = 2 * cap;
        if (c < m + 1) c = m + 1;
        E_tab = grow_zero(E_tab, cap, c);
        prev_row = grow_zero(prev_row, cap, c);
        curr_row = grow_zero(curr_row, cap, c);
        cap = c;
    }
    for (idx_t row = done + 1; row <= m; row++) {
        curr_row[0] = 0;
        for (idx_t k = 1; k <= row; k++) {
            curr_row[k] = curr_row[k - 1] + prev_row[row - k];
        }
        E_tab[row] = curr_row[row];
        seq_t *t = prev_row;
        prev_row = curr_row;
        curr_row = t;
        done = row;
    }
}

seq_t a(idx_t n) {
    ensure(n + 2);
    return 3 * E_tab[n + 2] - E_tab[n];
}
