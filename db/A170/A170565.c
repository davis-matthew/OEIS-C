/*
 * A170565  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170565
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

/*  OEIS A170565
 *  G.f. (t^47 + 2*t^46 + 2*t^45 + ... + 2*t + 1)/(351*t^47 - 26*t^46 - 26*t^45 - ... - 26*t - 1)
 *
 *  The coefficients are obtained by dividing the two polynomials.
 *  This is equivalent to the linear recurrence
 *      a(0) = 1
 *      for 1 ≤ n ≤ 46 :   a(n) = 2 + 26 · Σ_{k=0}^{n-1} a(k)
 *      for n = 47      :   a(n) = 1 + 26 · Σ_{k=1}^{46} a(k) - 351 · a(0)
 *      for n ≥ 48      :   a(n) = 26 · Σ_{k=n-46}^{n-1} a(k) - 351 · a(n-47)
 *
 *  This implementation uses a rolling window of size 46 to keep the
 *  running sum needed for the recurrence.  All arithmetic is performed
 *  in the build‑specific type seq_t; intermediate values are stored
 *  in the same type, so the compiler may apply modulo reduction if
 *  seq_t is an unsigned integer type.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case immediately. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate storage for the sequence up to index n. */
    seq_t *seq = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!seq) {
        return (seq_t)0;         /* allocation failure */
    }

    /* Initial term */
    seq[0] = (seq_t)1;

    /* windowSum holds the sum of the most recent up to 46 terms.
       Initially it contains a[0] for the first iteration (n = 1). */
    seq_t windowSum = seq[0];

    for (idx_t i = 1; i <= n; ++i) {
        /* Determine N_i, the coefficient of t^i in the numerator. */
        seq_t Ni;
        if (i <= 46) {
            Ni = (seq_t)2;
        } else if (i == 47) {
            Ni = (seq_t)1;
        } else {
            Ni = 0;
        }

        /* Compute the new term using the recurrence. */
        seq_t term = Ni + (seq_t)26 * windowSum;
        if (i >= 47) {
            term -= (seq_t)351 * seq[i - 47];
        }

        seq[i] = term;

        /* Update the rolling window for the next iteration. */
        if (i >= 46) {
            windowSum -= seq[i - 46];
        }
        windowSum += seq[i];
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
