/*
 * A143377  Expansion of q^(-1/6) * eta(q)^2 * eta(q^4) / eta(q^2) in powers
 *          of q.
 * https://oeis.org/A143377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k>0} (1 - x^k)^2 * (1 + x^(2k))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) by expanding the product up to the required index.
   This uses O(n^2) time and O(n) memory, which is acceptable for n up to ~10 000. */
seq_t a(idx_t n)
{
    /* Allocate coefficient arrays.  A holds the current polynomial,
       B holds the next polynomial after processing a new k. */
    seq_t *A = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *B = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!A || !B) {          /* allocation failure – return 0 as fallback */
        free(A);
        free(B);
        return (seq_t)0;
    }

    /* The identity polynomial starts as 1.  Remaining entries are already zero. */
    A[0] = (seq_t)1;

    /* Process each factor (1 - 2x^k + 2x^{2k} - 2x^{3k} + x^{4k}) for k = 1 .. n. */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t j = 0; j <= n; ++j) {
            seq_t val = A[j];
            if (j >= k)     val -= (seq_t)2 * A[j - k];
            if (j >= 2 * k) val += (seq_t)2 * A[j - 2 * k];
            if (j >= 3 * k) val -= (seq_t)2 * A[j - 3 * k];
            if (j >= 4 * k) val += A[j - 4 * k];
            B[j] = val;
        }
        /* Swap A and B for the next iteration. */
        seq_t *tmp = A; A = B; B = tmp;
    }

    seq_t result = A[n];

    free(A);
    free(B);
    return result;
}
