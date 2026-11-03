/*
 * A079902  G.f. = { 1+sum(4*n*q^n, n=1..infinity)} / { theta series for
 *          square lattice }.
 * https://oeis.org/A079902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in (1+4∑_{k≥1} k q^k) / (∑_{m≥0}
 *            r_2(m) q^m) Recurrence: a(0)=1; for n>0, a(n)=4n – Σ_{k=1..n}
 *            r_2(k)·a(n−k), where r_2(k)=4·(d1(k)−d3(k)) and d1,d3 count
 *            divisors ≡1,3 (mod 4)
 * Range:     i8 n <= 8, i16 n <= 20, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 38 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t, NULL */
#include "../utils/mem.h"

static seq_t r2_func(idx_t n) {
    if (n == 0) return (seq_t)1;
    idx_t d;
    int cnt1 = 0, cnt3 = 0;
    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            if (d1 % 4 == 1) ++cnt1;
            else if (d1 % 4 == 3) ++cnt3;
            idx_t d2 = n / d;
            if (d2 != d1) {
                if (d2 % 4 == 1) ++cnt1;
                else if (d2 % 4 == 3) ++cnt3;
            }
        }
    }
    return (seq_t)((seq_t)4 * ((seq_t)cnt1 - (seq_t)cnt3));
}

seq_t a(idx_t n) {
    static seq_t *D = NULL;   /* r_2 coefficients */
    static seq_t *A = NULL;   /* sequence values    */
    static size_t cap = 0;    /* current capacity   */

    size_t N = (size_t)n;
    if (N >= cap) {
        size_t new_cap = N + 1;
        seq_t *newD = mem_new(seq_t, new_cap);
        seq_t *newA = mem_new(seq_t, new_cap);

        size_t i;
        /* copy old data */
        for (i = 0; i < cap; ++i) {
            newD[i] = D[i];
            newA[i] = A[i];
        }

        /* compute new entries */
        for (i = cap; i <= N; ++i) {
            newD[i] = r2_func((idx_t)i);
            if (i == 0) {
                newA[i] = (seq_t)1;
            } else {
                seq_t sum = 0;
                size_t j;
                for (j = 0; j < i; ++j) {
                    sum += newD[i - j] * newA[j];
                }
                seq_t Ncoeff = (seq_t)4 * (seq_t)i;
                newA[i] = Ncoeff - sum;
            }
        }

        mem_free(D);
        mem_free(A);
        D = newD;
        A = newA;
        cap = new_cap;
    }
    return A[N];
}
