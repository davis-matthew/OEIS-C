/*
 * A007548  Shifts 3 places left under exponentiation.
 * https://oeis.org/A007548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   E.g.f. A(x) satisfies A'''(x) = exp(A(x)), so a(n+3) = b(n)
 *            where exp(A(x)) = Sum b(n) x^n/n!. From B' = A'B: a(n) =
 *            Sum_{k=0..n-4} binomial(n-4,k) * a(k+1) * a(n-1-k) for n >= 4,
 *            a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 18, u64 n <= 27, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 196 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *A7548_vals = NULL;
static seq_t *A7548_row = NULL;
static idx_t A7548_cap = 0;
static idx_t A7548_done = 0;
static idx_t A7548_rowm = 0;

seq_t a(idx_t n) {
    if (n <= 3) return 1;

    if (n + 1 > A7548_cap) {
        idx_t newcap = 2 * A7548_cap;
        if (newcap < n + 1) newcap = n + 1;
        if (newcap < 8) newcap = 8;
        A7548_vals = grow_zero(A7548_vals, A7548_cap, newcap);
        A7548_row = grow_zero(A7548_row, A7548_cap, newcap);
        A7548_cap = newcap;
    }

    if (A7548_done == 0) {
        A7548_vals[1] = 1;
        A7548_vals[2] = 1;
        A7548_vals[3] = 1;
        A7548_row[0] = 1;
        A7548_rowm = 0;
        A7548_done = 3;
    }

    for (idx_t i = A7548_done + 1; i <= n; i++) {
        idx_t m = i - 4;
        while (A7548_rowm < m) {
            A7548_rowm++;
            A7548_row[A7548_rowm] = 1;
            for (idx_t k = A7548_rowm - 1; k >= 1; k--)
                A7548_row[k] += A7548_row[k - 1];
        }
        seq_t sum = 0;
        for (idx_t k = 0; k <= m; k++)
            sum += A7548_row[k] * A7548_vals[k + 1] * A7548_vals[i - 1 - k];
        A7548_vals[i] = sum;
        A7548_done = i;
    }

    return A7548_vals[n];
}
