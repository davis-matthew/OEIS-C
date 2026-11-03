/*
 * A002288  G.f.: q * Product_{m>=1} (1-q^m)^8*(1-q^2m)^8.
 * https://oeis.org/A002288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   a(0) = 0; for n >= 1, a(n) = Sum_{i=0..n-1} Q(i)*Q(n-1-i), where
 *            Q(q) = Product_{m>=1} (1-q^m)^4*(1-q^(2m))^4 (A030211), built as
 *            E(q)^3 * E(q) * E(q^2)^3 * E(q^2) with E(q) = Product (1-q^m):
 *            E(q)^3 = Sum_{k>=0} (-1)^k (2k+1) q^(k(k+1)/2) (Jacobi), E(q) =
 *            Sum_k (-1)^k q^(k(3k-1)/2) (Euler)
 * Range:     i8 n <= 4, i16 n <= 18, i32 n <= 412, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 8629 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* multiply T (length L) in place by E(q^s)^3 */
static void mul_cube(seq_t *T, idx_t L, idx_t s) {
    for (idx_t i = L; i-- > 0;) {
        seq_t acc = T[i];
        for (idx_t k = 1;; k++) {
            idx_t e = s * (k * (k + 1) / 2);
            if (e > i) break;
            if (k & 1) acc -= T[i - e] * (seq_t)(2 * k + 1);
            else       acc += T[i - e] * (seq_t)(2 * k + 1);
        }
        T[i] = acc;
    }
}

/* multiply T (length L) in place by E(q^s) */
static void mul_pent(seq_t *T, idx_t L, idx_t s) {
    for (idx_t i = L; i-- > 0;) {
        seq_t acc = T[i];
        for (idx_t k = 1;; k++) {
            idx_t e1 = s * (k * (3 * k - 1) / 2);
            if (e1 > i) break;
            idx_t e2 = s * (k * (3 * k + 1) / 2);
            if (k & 1) {
                acc -= T[i - e1];
                if (e2 <= i) acc -= T[i - e2];
            } else {
                acc += T[i - e1];
                if (e2 <= i) acc += T[i - e2];
            }
        }
        T[i] = acc;
    }
}

static seq_t *Qs = NULL;
static idx_t qlen = 0;

static void build(idx_t need) {
    idx_t L = need;
    if (qlen > 0 && 2 * qlen > L) L = 2 * qlen;
    seq_t *T = mem_new(seq_t, L);
    for (idx_t i = 0; i < L; i++) T[i] = 0;
    T[0] = 1;
    mul_cube(T, L, 1);
    mul_pent(T, L, 1);
    mul_cube(T, L, 2);
    mul_pent(T, L, 2);
    if (Qs) mem_free(Qs);
    Qs = T;
    qlen = L;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t N = n - 1;
    if (N + 1 > qlen) build(N + 1);
    seq_t s = 0;
    idx_t i = 0, j = N;
    while (i < j) {
        s += Qs[i] * Qs[j];
        i++;
        j--;
    }
    s = s * 2;
    if (i == j) s += Qs[i] * Qs[i];
    return s;
}
