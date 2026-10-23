/*
 * A161652  Number of reduced words of length n in the Weyl group A_39.
 * https://oeis.org/A161652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{39} (1 - x^(k+1))/(1 - x)
 *            Computation via the recurrence: Let G_0[0] = 1. For k = 1 .. 39:
 *            S[j] = sum_{i=0}^{j} G_{k-1}[i]; // prefix sums G_k[j] = S[j] -
 *            (j >= k+1 ? S[j-(k+1)] : 0); Then a(n) = G_39[n]. For n > 819
 *            the coefficient is zero
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Degree of the polynomial is sum_{k=1}^{39} (k+1) = 819 */
    const idx_t MAX_DEGREE = 819;
    if (n > MAX_DEGREE)
        return (seq_t)0;

    /* Allocate working arrays of length n+1. */
    size_t len = (size_t)n + 1;
    seq_t *curr  = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *next  = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *pref  = (seq_t *)calloc(len, sizeof(seq_t));
    if (!curr || !next || !pref) {
        /* In a real implementation you'd handle allocation failure. */
        return (seq_t)0;
    }

    curr[0] = (seq_t)1;  /* G_0[0] = 1 */

    for (idx_t k = 1; k <= 39; ++k) {
        /* Build prefix sums of curr */
        pref[0] = curr[0];
        for (size_t i = 1; i < len; ++i)
            pref[i] = pref[i - 1] + curr[i];

        /* Compute next using the recurrence */
        idx_t shift = k + 1;
        for (size_t i = 0; i < len; ++i) {
            if (i >= shift)
                next[i] = pref[i] - pref[i - shift];
            else
                next[i] = pref[i];
        }

        /* Prepare for next iteration */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    seq_t result = curr[n];

    free(curr);
    free(next);
    free(pref);

    return result;
}
