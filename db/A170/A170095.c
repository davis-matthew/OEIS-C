/*
 * A170095  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36 * sum_{i=1}^{36} a(n-i) - 666 * a(n-37) + c(n), where
 *            c(0)=1, c(1..36)=2, c(37)=1, c(n)=0 for n>37
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The recurrence is derived from the generating function 
    (t^37 + 2*t^36 + ... + 2*t + 1) / (1 - 36*t - 36*t^2 - ... - 36*t^36 + 666*t^37). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for terms up to n */
    size_t N = (size_t)n;
    seq_t *a = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (a == NULL) return (seq_t)0; /* Out of memory; caller should handle. */

    seq_t sum_prev36 = 0; /* Sum of the previous 36 values */

    for (size_t k = 0; k <= N; ++k)
    {
        seq_t term_n;
        if (k == 0)               term_n = 1;
        else if (k <= 36)         term_n = 2;
        else if (k == 37)         term_n = 1;
        else                      term_n = 0;

        seq_t prev37 = (k >= 37) ? a[k - 37] : 0;

        a[k] = term_n + 36 * sum_prev36 - 666 * prev37;

        /* Update running sum for the next iteration */
        sum_prev36 += a[k];
        if (k >= 36) sum_prev36 -= a[k - 36];
    }

    seq_t result = a[N];
    free(a);
    return result;
}
