/*
 * A003576  Dowling numbers: e.g.f.: exp(x + (exp(b*x) - 1)/b) with b=4.
 * https://oeis.org/A003576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), where T(n,k) = T(n-1,k-1) +
 *            (4*k+1)*T(n-1,k), T(0,0) = 1, T(n,k) = 0 for k < 0 or k > n
 *            (from a(n) = exp(-1/4) * Sum_{k>=0} (4k+1)^n/(4^k k!))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   A3576_regrow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A3576_regrow.h"

static seq_t *A3576_row = NULL;   /* current triangle row T(cur, 0..cur) */
static seq_t *A3576_vals = NULL;  /* a(0..cur) */
static idx_t A3576_cur = 0;
static idx_t A3576_cap = 0;

seq_t a(idx_t n) {
    idx_t i, k;
    if (A3576_cap == 0) {
        A3576_cap = 64;
        A3576_row = A3576_regrow(NULL, 0, A3576_cap);
        A3576_vals = A3576_regrow(NULL, 0, A3576_cap);
        A3576_row[0] = 1;
        A3576_vals[0] = 1;
        A3576_cur = 0;
    }
    if (n <= A3576_cur) return A3576_vals[n];
    if (n + 1 > A3576_cap) {
        idx_t nc = A3576_cap;
        while (nc < n + 1) nc *= 2;
        A3576_row = A3576_regrow(A3576_row, A3576_cur + 1, nc);
        A3576_vals = A3576_regrow(A3576_vals, A3576_cur + 1, nc);
        A3576_cap = nc;
    }
    for (i = A3576_cur + 1; i <= n; i++) {
        seq_t s;
        A3576_row[i] = 0;
        for (k = i; k >= 1; k--) {
            A3576_row[k] = A3576_row[k - 1] + (seq_t)(4 * k + 1) * A3576_row[k];
        }
        /* T(i,0) = 1 stays unchanged */
        s = 0;
        for (k = 0; k <= i; k++) s += A3576_row[k];
        A3576_vals[i] = s;
    }
    A3576_cur = n;
    return A3576_vals[n];
}
