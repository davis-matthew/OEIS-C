/*
 * A169249  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 42*Σ_{k=1}^{27} a(n-k) – 903*a(n-28) + p_n,
   where p_0 = 1, p_1 … p_27 = 2, p_28 = 1, p_n = 0 for n>28.
   Initial values: a(0)=1, a(1)=44, a(2)=1892, … (see OEIS A169249) */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate array for values a[0 .. n] */
    seq_t *a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a) return (seq_t)0;        /* out of memory */

    a[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Sum of up to the previous 27 terms */
        seq_t sum = (seq_t)0;
        idx_t start = (i > 27) ? (i - 27) : 0;
        for (idx_t j = start; j < i; ++j)
            sum += a[j];

        seq_t term = mul_small(sum, 42);  /* 42 * Σ a(i-k) */

        /* Add inhomogeneous term p_i */
        if (i <= 27)
            term += (seq_t)2;
        else if (i == 28)
            term += (seq_t)1;

        /* Subtract 903*a(i-28) if i >= 28 */
        if (i >= 28)
            term -= mul_small(a[i - 28], 903);

        a[i] = term;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
