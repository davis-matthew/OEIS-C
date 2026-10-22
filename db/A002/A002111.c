/*
 * A002111  Glaisher's G numbers.
 * https://oeis.org/A002111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   E.g.f. 3*x/(2+4*cos(x)) = Sum a(n)*x^(2n+1)/(2n+1)!, with
 *            a(0)=1/2, gives the recurrence 3*a(k) = 2*Sum_{j=1..k-1}
 *            (-1)^(j+1)*C(2k+1,2j)*a(k-j) + (-1)^(k+1)*(2k+1), for k >= 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/grow_zero.h"

static seq_t *A = NULL;     /* A[1..have] */
static seq_t *row = NULL;   /* Pascal row R = 2*have+1, entries 0..R */
static idx_t have = 0;
static idx_t cap = 0;       /* capacity of A */
static idx_t rcap = 0;      /* capacity of row */
static int fixed = -1;
static seq_t inv3;

static void advance_row(idx_t R) {
    /* row holds row R; make it row R+1 */
    row[R + 1] = 1;
    for (idx_t i = R; i >= 1; i--)
        row[i] = row[i] + row[i - 1];
}

seq_t a(idx_t n) {
    if (fixed < 0) {
        seq_t t = 1;
        int i;
        fixed = 0;
        for (i = 0; i < 300; i++) {
            t = t + t;
            if (t == 0) { fixed = 1; break; }
        }
        if (fixed) {
            seq_t x = 3;
            for (i = 0; i < 8; i++)
                x = x * ((seq_t)2 - x * 3);
            inv3 = x;
        }
    }
    if (n <= have) return A[n];

    if (n + 1 > cap) {
        idx_t nc = (n + 1 > 2 * cap) ? n + 1 : 2 * cap;
        A = grow_zero(A, cap, nc);
        cap = nc;
    }
    if (2 * n + 2 > rcap) {
        idx_t nr = 2 * cap + 2;
        row = grow_zero(row, rcap, nr);
        if (rcap == 0) { row[0] = 1; row[1] = 1; }
        rcap = nr;
    }

    for (idx_t k = have + 1; k <= n; k++) {
        idx_t R = 2 * (k - 1) + 1;
        advance_row(R);
        advance_row(R + 1);
        /* row now holds C(2k+1, .) */
        seq_t P = 0, N = 0;
        for (idx_t j = 1; j < k; j++) {
            seq_t t = row[2 * j] * A[k - j];
            t += t;
            if (j & 1) P += t; else N += t;
        }
        if (k & 1) P += (seq_t)(2 * k + 1);
        else N += (seq_t)(2 * k + 1);
        seq_t S = P - N;
        if (fixed) A[k] = S * inv3;
        else A[k] = S / 3;
        have = k;
    }
    return A[n];
}
