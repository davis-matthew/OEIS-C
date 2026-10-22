/*
 * A000657  Median Euler numbers (the middle numbers of Arnold's shuttle
 *          triangle).
 * https://oeis.org/A000657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = E(2n, n), the middle entry of row 2n of the Entringer
 *            (Seidel-Entringer-Arnold) triangle: E(0,0) = 1, E(m,0) = 0 for m
 *            > 0, E(m,k) = E(m,k-1) + E(m-1,m-k) (central terms of A008280)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"

static seq_t *cur = NULL;   /* current row m, entries 0..m */
static seq_t *nxt = NULL;
static idx_t row_cap = 0;
static idx_t m_cur = 0;
static seq_t *memo = NULL;
static idx_t memo_cap = 0;
static idx_t memo_len = 0;

static seq_t *grow_buf(seq_t *old, idx_t oldlen, idx_t newcap) {
    seq_t *p = alloc_zero(newcap);
    for (idx_t i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) free(old);
    return p;
}

seq_t a(idx_t n) {
    if (memo_len == 0) {
        row_cap = 16;
        cur = alloc_zero(row_cap);
        nxt = alloc_zero(row_cap);
        cur[0] = 1;
        m_cur = 0;
        memo_cap = 16;
        memo = alloc_zero(memo_cap);
        memo[0] = 1;
        memo_len = 1;
    }
    if (n < memo_len) return memo[n];

    idx_t need_row = 2 * n + 2;
    if (need_row > row_cap) {
        idx_t nc = row_cap * 2;
        if (nc < need_row) nc = need_row;
        cur = grow_buf(cur, m_cur + 1, nc);
        free(nxt);
        nxt = alloc_zero(nc);
        row_cap = nc;
    }
    if (n + 1 > memo_cap) {
        idx_t nc = memo_cap * 2;
        if (nc < n + 1) nc = n + 1;
        memo = grow_buf(memo, memo_len, nc);
        memo_cap = nc;
    }

    while (m_cur < 2 * n) {
        idx_t m = m_cur;
        nxt[0] = 0;
        for (idx_t k = 1; k <= m + 1; k++) {
            nxt[k] = nxt[k - 1] + cur[m + 1 - k];
        }
        seq_t *t = cur; cur = nxt; nxt = t;
        m_cur = m + 1;
        if ((m_cur & 1) == 0) {
            memo[m_cur / 2] = cur[m_cur / 2];
            memo_len = m_cur / 2 + 1;
        }
    }
    return memo[n];
}
