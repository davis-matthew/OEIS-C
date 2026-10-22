/*
 * A001028  E.g.f. A(x) satisfies A'(x) = 1 + A(A(x)), with A(0) = 0.
 * https://oeis.org/A001028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen, nice
 * Formula:   a(1) = 1, a(n+1) = Sum_{k=1..n} a(k) * B(n,k), where B(n,k) =
 *            n!*[x^n] A(x)^k/k! are the partial Bell polynomials in a(1),
 *            a(2), ...: B(0,0) = 1, B(n,0) = 0 (n>0), B(n,k) =
 *            Sum_{j=1..n-k+1} binomial(n-1,j-1) * a(j) * B(n-j,k-1). (From
 *            A'(x) = 1 + A(A(x)): a(n+1) = n! [x^n] A(A(x)) for n >= 1.)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 28,
 *            big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/a001028_grow.h"
#include "../utils/a001028_z.h"

static seq_t *a001028_A = NULL;     /* a(1..known) */
static seq_t *a001028_P = NULL;     /* current Pascal row */
static seq_t *a001028_ca = NULL;    /* scratch: binomial * a(j) */
static seq_t **a001028_B = NULL;    /* B rows 0..done */
static idx_t a001028_cap = 0;
static idx_t a001028_done = 0;      /* last computed B row */
static idx_t a001028_known = 0;     /* last known a index */
static idx_t a001028_prow = 0;      /* index of current Pascal row */

static void a001028_ensure(idx_t need) {
    if (need + 2 <= a001028_cap) return;
    idx_t nc = a001028_cap * 2;
    if (nc < need + 2) nc = need + 2;
    a001028_A = a001028_grow(a001028_A, a001028_cap, nc);
    a001028_P = a001028_grow(a001028_P, a001028_cap, nc);
    a001028_ca = a001028_grow(a001028_ca, a001028_cap, nc);
    seq_t **nb = (seq_t **)realloc(a001028_B, (size_t)nc * sizeof(seq_t *));
    for (idx_t i = a001028_cap; i < nc; i++) nb[i] = NULL;
    a001028_B = nb;
    a001028_cap = nc;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a001028_cap == 0) {
        a001028_ensure(n < 8 ? 8 : n);
        a001028_A[1] = 1;
        a001028_B[0] = a001028_z(1);
        a001028_B[0][0] = 1;
        a001028_P[0] = 1;
        a001028_prow = 0;
        a001028_done = 0;
        a001028_known = 1;
    }
    if (n <= a001028_known) return a001028_A[n];
    a001028_ensure(n);

    while (a001028_known < n) {
        idx_t r = a001028_done + 1;
        /* advance Pascal row to r-1 */
        while (a001028_prow < r - 1) {
            a001028_prow++;
            for (idx_t j = a001028_prow; j >= 1; j--)
                a001028_P[j] += a001028_P[j - 1];
        }
        for (idx_t j = 1; j <= r; j++)
            a001028_ca[j] = a001028_P[j - 1] * a001028_A[j];

        seq_t *row = a001028_z(r + 1);
        row[0] = 0;
        for (idx_t k = 1; k <= r; k++) {
            seq_t s = 0;
            for (idx_t j = 1; j <= r - k + 1; j++)
                s += a001028_ca[j] * a001028_B[r - j][k - 1];
            row[k] = s;
        }
        a001028_B[r] = row;
        a001028_done = r;

        seq_t t = 0;
        for (idx_t k = 1; k <= r; k++)
            t += a001028_A[k] * row[k];
        a001028_A[r + 1] = t;
        a001028_known = r + 1;
    }
    return a001028_A[n];
}
