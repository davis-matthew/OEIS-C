/*
 * A006698  T(2,2n), where T(k,m) is the number of sequences a_1,...,a_m of
 *          integers 0,1,...,n with n=floor(m/k) such that the 'bumped'
 *          sequence b_1,...,b_m has exactly k of each of 0,...,n-1, where
 *          b_i=a_i + j (mod n+1) with minimal j>=0 such that b_0,...,b_i
 *          contain at most k elements equal to b_i.
 * https://oeis.org/A006698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the bumped sequence ends with exactly 2 of each of 0..n-1, so no
 *            a_i can equal n. That makes a(n) the number of words of length
 *            2n over {0..n-1} with N_j = #{a_i < j} >= 2j. Split by the first
 *            j where this fails: B(n) = n^(2n-1) - Sum_{i=1..n-1}
 *            [C(2n-1,2i-2)*a(i-1)*(n-i)^(2n-2i+1) + C(2n-1,2i-1)...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *A_tab = 0, *B_tab = 0;
static idx_t tab_len = 0, tab_cap = 0;
static seq_t *row = 0;
static idx_t row_m = 0, row_cap = 0;

/* make row hold C(m, k), k = 0..m */
static void advance_row(idx_t m) {
    if (!row) {
        row_cap = 16;
        row = alloc_zero(row_cap);
        row[0] = 1;
        row_m = 0;
    }
    while (row_m < m) {
        idx_t r = row_m + 1, k;
        if (r + 1 > row_cap) {
            idx_t nc = row_cap * 2;
            while (nc < r + 1) nc *= 2;
            row = grow_zero(row, row_cap, nc);
            row_cap = nc;
        }
        for (k = r; k >= 1; k--) row[k] += row[k - 1];
        row_m = r;
    }
}

static void ensure_tab(idx_t len) {
    if (len > tab_cap) {
        idx_t nc = tab_cap ? tab_cap : 16;
        while (nc < len) nc *= 2;
        A_tab = grow_zero(A_tab, tab_cap, nc);
        B_tab = grow_zero(B_tab, tab_cap, nc);
        tab_cap = nc;
    }
}

seq_t a(idx_t n) {
    ensure_tab(n + 1);
    while (tab_len <= n) {
        idx_t N = tab_len, i;
        if (N == 0) {
            A_tab[0] = 1;
            B_tab[0] = 0;
            tab_len = 1;
            continue;
        }
        /* powers (N-i)^(2(N-i)) for i = 1..N-1 */
        seq_t *pows = alloc_zero(N);
        for (i = 1; i < N; i++) {
            idx_t d = N - i;
            pows[i] = ipow_idx((seq_t)d, 2 * d);
        }
        /* B(N) uses row 2N-1 */
        advance_row(2 * N - 1);
        {
            seq_t s = 0;
            for (i = 1; i < N; i++) {
                seq_t d = (seq_t)(N - i);
                seq_t p = pows[i];
                s += row[2 * i - 2] * A_tab[i - 1] * (p * d);
                s += row[2 * i - 1] * B_tab[i] * p;
            }
            B_tab[N] = ipow_idx((seq_t)N, 2 * N - 1) - s;
        }
        /* a(N) uses row 2N */
        advance_row(2 * N);
        {
            seq_t s = 0;
            for (i = 1; i < N; i++) {
                seq_t d = (seq_t)(N - i);
                seq_t p = pows[i];
                s += row[2 * i - 2] * A_tab[i - 1] * (p * d * d);
                s += row[2 * i - 1] * B_tab[i] * (p * d);
            }
            A_tab[N] = ipow_idx((seq_t)N, 2 * N) - s;
        }
        mem_free(pows);
        tab_len = N + 1;
    }
    return A_tab[n];
}
