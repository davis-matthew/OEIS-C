/*
 * A211371  The number of indecomposable n-permutations that have only cycles
 *          of length 3 or less.
 * https://oeis.org/A211371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n) - Σ_{k=1}^{n-1} A(k) * T(n-k) where T(n) is the
 *            number of permutations of n with all cycles of length ≤ 3,
 *            defined by T(0)=1, T(n)=T(n-1)+ (n-1)*T(n-2)+ (n-1)(n-2)*T(n-3)
 *            for n≥1. a(0)=0. See A211371
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation tables for total (T) and indecomposable (A) counts. */
    static seq_t *T = NULL;
    static seq_t *A = NULL;
    static idx_t cap = 0;   /* current allocated capacity */

    /* Handle the trivial case first. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Extend the tables if required. */
    if (n >= cap)
    {
        idx_t new_cap = n + 1;
        T = (seq_t *)realloc(T, new_cap * sizeof(seq_t));
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));

        /* Fill values from the old capacity up to the new one. */
        for (idx_t i = cap; i < new_cap; ++i)
        {
            if (i == 0)
            {
                T[i] = (seq_t)1;   /* T(0)=1 */
                A[i] = (seq_t)0;   /* A(0)=0 */
                continue;
            }

            /* Compute T(i) using the recurrence. */
            seq_t Ti = (seq_t)0;

            /* k = 1 term */
            Ti = Ti + T[i - 1];

            /* k = 2 term, valid for i ≥ 2 */
            if (i >= 2)
            {
                seq_t fac2 = (seq_t)(i - 1);             /* (i-1) */
                Ti = Ti + fac2 * T[i - 2];
            }

            /* k = 3 term, valid for i ≥ 3 */
            if (i >= 3)
            {
                seq_t fac3 = (seq_t)((i - 1) * (i - 2)); /* (i-1)(i-2) */
                Ti = Ti + fac3 * T[i - 3];
            }

            T[i] = Ti;

            /* Compute A(i) = T(i) - Σ_{k=1}^{i-1} A(k) * T(i-k). */
            seq_t sub = (seq_t)0;
            for (idx_t k = 1; k < i; ++k)
                sub = sub + A[k] * T[i - k];

            A[i] = T[i] - sub;
        }

        cap = new_cap;
    }

    return A[n];
}
