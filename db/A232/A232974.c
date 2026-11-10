/*
 * A232974  Number of irreducible packed words of length n.
 * https://oeis.org/A232974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   g.f. A(x) = 2 - 1/F(x), F = g.f. of A000629; so a(0)=1, a(n) =
 *            A000629(n) - Sum_{k=1..n-1} A000629(k)*a(n-k) for n>=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A000629_row = NULL;
static seq_t *A000629_vals = NULL;
static idx_t A000629_done = 0;
static idx_t A000629_cap = 0;

static seq_t *A000629_grow(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *r = mem_new(seq_t, (size_t)newlen);
    idx_t i;
    for (i = 0; i < oldlen; i++) r[i] = old[i];
    if (old) mem_free(old);
    return r;
}

static void A000629_ensure(idx_t need) {
    idx_t newcap;
    if (A000629_cap >= need) return;
    newcap = A000629_cap * 2;
    if (newcap < need) newcap = need;
    if (newcap < 64) newcap = 64;
    A000629_row = A000629_grow(A000629_row, A000629_cap, newcap);
    A000629_vals = A000629_grow(A000629_vals, A000629_cap, newcap);
    A000629_cap = newcap;
}

static seq_t A000629_a(idx_t n) {
    A000629_ensure(n + 2);
    if (A000629_done == 0) {
        A000629_row[0] = (seq_t)1;
        A000629_vals[0] = (seq_t)1;
        A000629_done = 1;
    }
    while (A000629_done <= n) {
        idx_t r = A000629_done;
        idx_t k;
        seq_t sum = (seq_t)0;
        A000629_row[r] = (seq_t)0;
        for (k = r; k >= 1; k--) {
            seq_t t = A000629_row[k] + A000629_row[k - 1];
            A000629_row[k] = (seq_t)k * t;
        }
        A000629_row[0] = (seq_t)0;
        for (k = 1; k <= r; k++) sum += A000629_row[k];
        A000629_vals[r] = sum + sum;
        A000629_done++;
    }
    return A000629_vals[n];
}

static seq_t *A232974_memo = NULL;
static idx_t A232974_done = 0;
static idx_t A232974_cap = 0;

seq_t a(idx_t n) {
    if (A232974_cap < n + 1) {
        idx_t nc = A232974_cap * 2;
        idx_t i;
        seq_t *p;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        p = mem_new(seq_t, (size_t)nc);
        for (i = 0; i < A232974_done; i++) p[i] = A232974_memo[i];
        if (A232974_memo) mem_free(A232974_memo);
        A232974_memo = p;
        A232974_cap = nc;
    }
    if (A232974_done == 0) {
        A232974_memo[0] = (seq_t)1;
        A232974_done = 1;
    }
    while (A232974_done <= n) {
        idx_t m = A232974_done;
        idx_t k;
        seq_t s = (seq_t)0;
        for (k = 1; k < m; k++) {
            s += A000629_a(k) * A232974_memo[m - k];
        }
        A232974_memo[m] = A000629_a(m) - s;
        A232974_done++;
    }
    return A232974_memo[n];
}
