/*
 * A006677  Number of planted binary phylogenetic trees with n labels.
 * https://oeis.org/A006677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = 1 + Sum_{0<k<n} binomial(n-1,k)*a(k)*a(n-k), a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 348 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

static seq_t *A_memo = NULL;   /* A_memo[m] = a(m) for 1 <= m <= done */
static seq_t *row = NULL;      /* row[k] = binomial(done-1, k) */
static idx_t done = 0;
static idx_t cap = 0;

static void ensure_cap(idx_t need) {
    idx_t newcap;
    if (need <= cap) return;
    newcap = cap ? cap : 16;
    while (newcap < need) newcap *= 2;
    if (A_memo == NULL) {
        A_memo = alloc_zero(newcap);
        row = alloc_zero(newcap);
    } else {
        A_memo = grow_zero(A_memo, cap, newcap);
        row = grow_zero(row, cap, newcap);
    }
    cap = newcap;
}

seq_t a(idx_t n) {
    idx_t m, k;
    if (n < 1) return 0;
    ensure_cap(n + 2);
    if (done == 0) {
        row[0] = 1;            /* binomial(0,0) */
        A_memo[1] = 1;
        done = 1;
    }
    for (m = done + 1; m <= n; m++) {
        seq_t s;
        /* advance Pascal row from binomial(m-2,.) to binomial(m-1,.) */
        for (k = m - 1; k >= 1; k--)
            row[k] += row[k - 1];
        s = 1;
        /* pair k and m-k: binom(m-1,k)+binom(m-1,m-k) */
        for (k = 1; 2 * k < m; k++)
            s += (row[k] + row[m - k]) * A_memo[k] * A_memo[m - k];
        if (m % 2 == 0)
            s += row[m / 2] * A_memo[m / 2] * A_memo[m / 2];
        A_memo[m] = s;
        done = m;
    }
    return A_memo[n];
}
