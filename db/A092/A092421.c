/*
 * A092421  Coefficients of the solution to a functional equation.
 * https://oeis.org/A092421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 32, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the functional equations:
   A(x^2) = -A(x)*A(-x) and
   -A(x)^2 + A(x^2) + 6*A(x)A(x^2) = 0.
   For k ≥ 2 let n = k+1. Then
     a_k = ( a_{⌊n/2⌋}
            + 6*∑_{j=1}^{⌊n/2⌋} a_{n-2j} * a_j
            - ∑_{i=2}^{k-1} a_i * a_{n-i} ) / 2,
   with a_1 = 1. This recursion generates the sequence. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *a = NULL;
    static idx_t size = 0;

    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* initialise a[1] */
    if (size < 1) {
        a = (seq_t *)malloc(2 * sizeof(seq_t));
        a[1] = 1;
        size = 1;
    }
    if (n <= size) return a[n];

    /* grow array to contain n+1 entries (index 0 unused) */
    a = (seq_t *)realloc(a, (n + 1) * sizeof(seq_t));

    for (idx_t k = size + 1; k <= n; ++k) {
        idx_t eqN = k + 1;          /* equation index */
        seq_t C = 0;
        if ((eqN & 1) == 0) {       /* even eqN */
            idx_t mid = eqN >> 1;   /* (k+1)/2 */
            C = a[mid];
        }

        /* S2 = Σ_{j=1}^{⌊eqN/2⌋} a_{eqN-2j} * a_j */
        seq_t S2 = 0;
        for (idx_t j = 1; j <= (eqN >> 1); ++j) {
            idx_t i = eqN - 2 * j;
            if (i < 1) break;
            S2 += a[i] * a[j];
        }

        /* sum_terms = Σ_{i=2}^{k-1} a_i * a_{eqN-i} */
        seq_t sum_terms = 0;
        for (idx_t i = 2; i <= k - 1; ++i) {
            sum_terms += a[i] * a[eqN - i];
        }

        seq_t numerator = C + 6 * S2 - sum_terms;
        a[k] = numerator / 2;      /* a[1] = 1; division is exact */
    }

    size = n;
    return a[n];
}
