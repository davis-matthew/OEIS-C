/*
 * A169199  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function for A169199.
   a(0) = 1
   For n ≥ 1:
     a(n) = Σ_{k=1}^{26} 40·a(n‑k)
            – (n ≥ 27 ? 820·a(n‑27) : 0)
            + p(n)
   where the numerator coefficients p(n) are
     p(0)   = 1
     p(1…26)= 2
     p(27)  = 1
     p(n)   = 0 for n ≥ 28                                 */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for all needed terms.  */
    seq_t *A = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!A) abort();                       /* memory error – stop */

    A[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;

        /* Σ_{k=1}^{26} 40·a(i‑k) */
        for (idx_t k = 1; k <= 26 && i >= k; ++k)
            sum += (seq_t)40 * A[i - k];

        /* Subtract 820·a(i‑27) when applicable */
        if (i >= 27)
            sum -= (seq_t)820 * A[i - 27];

        /* Add the numerator coefficient p(i) */
        seq_t p;
        if (i == 0)
            p = 1;
        else if (i <= 26)
            p = 2;
        else if (i == 27)
            p = 1;
        else
            p = 0;

        sum += p;

        A[i] = sum;
    }

    seq_t result = A[n];
    free(A);
    return result;
}
