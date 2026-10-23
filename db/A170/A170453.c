/*
 * A170453  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The number of reduced words of length n in the Coxeter group on
 *            12 generators with relations (S_i)^2 = (S_i S_j)^45 = I. For 0 ≤
 *            n ≤ 44: a(n) = 12·11^(n-1) (with a(0)=1). For n ≥ 45 the
 *            sequence satisfies the linear recurrence a(n) = (2·∑_{k=1}^{44}
 *            a(n-k) – 11·a(n-45)) / 11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    /* Allocate storage for all terms up to n.  The helper returns a
       properly constructed array suitable for arbitrary‑precision
       types. */
    seq_t *A = seq_alloc_array(n + 1);
    if (!A) return (seq_t)0;   /* unlikely, but defensive */

    /* Initial terms a(0)=1, a(1)=12, a(i)=12·11^(i-1) for 2≤i≤44. */
    A[0] = (seq_t)1;
    if (n >= 1) A[1] = (seq_t)12;
    for (idx_t i = 2; i <= n && i <= 44; ++i)
        A[i] = A[i-1] * (seq_t)11;

    /* For n≥45 use the recurrence with a running sum of the last
       44 terms. */
    if (n >= 45) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 44; ++k) sum += A[k];

        for (idx_t i = 45; i <= n; ++i) {
            seq_t num = (seq_t)2 * sum - (seq_t)11 * A[i-45];
            A[i] = num / (seq_t)11;

            /* Update sum for the next step: subtract the term leaving
               the window and add the new term.  A[i-44] exists because
               i-44 ≥ 1 for i ≥ 45. */
            sum += A[i];
            sum -= A[i-44];
        }
    }

    seq_t result = A[n];
    /* The allocated array may be freed here; the build environment
       will reclaim memory after the function returns. */
    free(A);
    return result;
}
