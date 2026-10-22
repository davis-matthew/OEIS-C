/*
 * A003702  Expansion of tan(x)/cosh(x).
 * https://oeis.org/A003702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = T(2n+1) - Sum_{k=1..n} binomial(2n+1,2k)*a(n-k), a(0) =
 *            1, where T(m) = A000111(m) are the tangent numbers (from
 *            cosh(x)*f(x) = tan(x)), T computed by the Seidel-Entringer
 *            boustrophedon triangle
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *Ar = NULL;      /* memoized a(0..na-1) */
static idx_t na = 0, acap = 0;
static seq_t *E = NULL, *Et = NULL, *P = NULL;
static idx_t cap = 0;
static idx_t em = 0;          /* E holds Entringer row em */
static idx_t pm = 0;          /* P holds Pascal row pm */

static void ensure_cap(idx_t L) {
    if (L <= cap) return;
    idx_t nc = cap * 2;
    if (nc < L) nc = L;
    bool fresh = (cap == 0);
    E = grow_zero(E, cap, nc);
    Et = grow_zero(Et, cap, nc);
    P = grow_zero(P, cap, nc);
    cap = nc;
    if (fresh) {
        E[0] = 1; em = 0;
        P[0] = 1; pm = 0;
    }
}

seq_t a(idx_t n) {
    if (n < na) return Ar[n];
    if (n + 1 > acap) {
        idx_t nc = acap * 2;
        if (nc < n + 1) nc = n + 1;
        Ar = grow_zero(Ar, acap, nc);
        acap = nc;
    }
    ensure_cap(2 * n + 3);
    for (idx_t j = na; j <= n; j++) {
        idx_t m = 2 * j + 1;
        while (em < m) {
            Et[0] = 0;
            for (idx_t k = 1; k <= em + 1; k++)
                Et[k] = Et[k - 1] + E[em + 1 - k];
            seq_t *t = E; E = Et; Et = t;
            em++;
        }
        while (pm < m) {
            P[pm + 1] = 0;
            for (idx_t k = pm + 1; k >= 1; k--)
                P[k] += P[k - 1];
            pm++;
        }
        seq_t val = E[m];
        for (idx_t k = 1; k <= j; k++)
            val -= P[2 * k] * Ar[j - k];
        Ar[j] = val;
    }
    na = n + 1;
    return Ar[n];
}
