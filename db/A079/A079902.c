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
 * Helpers:   r2_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t, NULL */
#include "../utils/r2_func.h"

seq_t a(idx_t n) {
    static seq_t *D = NULL;   /* r_2 coefficients */
    static seq_t *A = NULL;   /* sequence values    */
    static size_t cap = 0;    /* current capacity   */

    size_t N = (size_t)n;
    if (N >= cap) {
        size_t new_cap = N + 1;
        seq_t *newD = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *newA = (seq_t *)malloc(new_cap * sizeof(seq_t));

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

        free(D);
        free(A);
        D = newD;
        A = newA;
        cap = new_cap;
    }
    return A[N];
}
