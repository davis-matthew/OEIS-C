/*
 * A170133  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170133: Number of reduced words of length n in the Coxeter
 * group on 28 generators with relations S_i^2 = (S_i S_j)^38 = I.
 *
 * The generating function is
 *   P(t)/Q(t) with
 *   P(t) = t^38 + 2Σ_{k=1}^{37} t^k + 1
 *   Q(t) = 1 - 26 t - 26 t^2 - … - 26 t^37 + 351 t^38
 *
 * From this rational form one obtains a linear recurrence:
 *
 *   a(0) = 1
 *   for 1 ≤ n ≤ 37  a(n) = 26 * Σ_{k=0}^{n-1} a(k) + 2
 *   a(38)      = 26 * Σ_{k=1}^{37} a(k) – 350
 *   for n ≥ 39  a(n) = 26 * Σ_{k=1}^{37} a(n-k) – 351 * a(n-38)
 *
 * The implementation below follows this recurrence.  All computations
 * are done in the type seq_t; intermediate sums are kept in seq_t
 * as well, guaranteeing exactness for all n ≥ 0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    /* Allocate array of length N+1.  seq_t may be a primitive type or an
       arbitrary‑precision integer class defined by the build system. */
    seq_t *A = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (A == NULL) return (seq_t)0;   /* out of memory; return 0 as safe default */

    seq_t cum = 0;          /* cumulative sum of a[0..i-1] */
    seq_t window = 0;       /* sum of the last 37 terms for recurrence */

    for (size_t i = 0; i <= N; ++i)
    {
        if (i == 0)
        {
            A[i] = (seq_t)1;
            cum  = A[i];
        }
        else if (i <= 37)
        {
            A[i] = (seq_t)26 * cum + (seq_t)2;
            cum  = cum + A[i];
        }
        else if (i == 38)
        {
            seq_t sum1 = cum - A[0];          /* Σ_{k=1}^{37} a(k) */
            A[i]      = (seq_t)26 * sum1 - (seq_t)350;
            cum       = cum + A[i];
        }
        else   /* i ≥ 39 */
        {
            if (i == 39)
            {
                /* Initialise window = Σ_{k=2}^{38} a(k) */
                window = 0;
                for (size_t k = i - 37; k < i; ++k)
                    window = window + A[k];
            }
            A[i] = (seq_t)26 * window
                   - (seq_t)351 * A[i - 38];
            /* Update window for next step: extend right, remove leftmost */
            window = window + A[i] - A[i - 37];
        }
    }

    seq_t result = A[N];
    free(A);
    return result;
}
