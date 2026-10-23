/*
 * A168804  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is defined by the linear recurrence
 *   a(0) = 1
 *   for 1 ≤ n ≤ 18:     a(n) = 2 + 29 * Σ_{k=0}^{n-1} a(k)
 *   for n = 19:         a(19) = 1 + 29 * Σ_{k=1}^{18} a(19-k) - 435 * a(0)
 *   for n ≥ 20:         a(n) = 29 * Σ_{k=1}^{18} a(n-k) - 435 * a(n-19)
 *
 * This recurrence follows from the rational generating function
 * (t^19 + 2*t^18 + … + 2*t + 1) /
 * (1 - 29*t - 29*t^2 - … - 29*t^18 + 435*t^19).
 * The sequence starts: 1, 31, 930, 27900, …
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0u)
        return (seq_t)1;

    /* Allocate array of length n+1 */
    seq_t *arr = (seq_t *)malloc((n + (idx_t)1) * sizeof(seq_t));
    if (!arr)        /* allocation failure – return 0 */
        return (seq_t)0;

    arr[0] = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        /* Compute Σ_{k=1}^{min(18,i)} a(i-k) */
        for (idx_t k = (idx_t)1; k <= (idx_t)18 && k <= i; ++k)
            sum += arr[i - k];

        /* RHS for n <= 18 is 2, for n == 19 is 1, else 0 */
        seq_t rhs;
        if (i <= (idx_t)18)
            rhs = (seq_t)2;
        else if (i == (idx_t)19)
            rhs = (seq_t)1;
        else
            rhs = (seq_t)0;

        seq_t val = rhs;

        /* Add 29 * sum. 29 is small, use repeated addition to avoid overflow. */
        for (idx_t k = (idx_t)1; k <= (idx_t)18 && k <= i; ++k)
        {
            seq_t part = arr[i - k];
            for (idx_t t = (idx_t)0; t < (idx_t)29; ++t)
                val += part;
        }

        /* Subtract 435 * a(i-19) when i >= 19 */
        if (i >= (idx_t)19)
        {
            seq_t part = arr[i - (idx_t)19];
            for (idx_t t = (idx_t)0; t < (idx_t)435; ++t)
                val -= part;
        }

        arr[i] = val;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
