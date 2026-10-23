/*
 * A135921  O.g.f.: A(x) = Sum_{n>=0} x^n / Product_{k=0..n} (1 - k*(k+1)*x).
 * https://oeis.org/A135921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135921
   a(n) is the coefficient of x^n in
   A(x) = Σ_{m≥0} x^m / Π_{k=0}^{m} (1 - k(k+1)x).
   The following implementation computes a(n) directly from this
   definition by iteratively building the coefficients of
   1/Π_{k=0}^{m} (1 - k(k+1)x).  The algorithm runs in Θ(n²) arithmetic
   operations and uses only integer arithmetic.  It works for any
   integer type seq_t supplied by the build (uint8_t…__int128 or
   an arbitrary‑precision integer class that supports the usual
   operators).
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequences are indexed from 0.  Allocate arrays of size n+1. */
    size_t N = (size_t)n;
    seq_t *a_arr   = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *coeff   = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *tmp     = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!a_arr || !coeff || !tmp) return (seq_t)0;   /* allocation failure */

    /* 1/P_{-1} = 1  → coeff[0] = 1 */
    coeff[0] = (seq_t)1;

    for (size_t k = 0; k <= N; ++k)
    {
        /* Factor for the current term: 1 / (1 - k(k+1) x)  */
        seq_t c = (seq_t)k * (seq_t)(k + 1);

        size_t max_deg = N - k;      /* we only need coefficients up to this degree */

        /* Compute new coefficients for 1/P_k from old coeff[ ].
           Using the recurrence: new[m] = coeff[m] + c * new[m-1].
         */
        tmp[0] = coeff[0];
        for (size_t m = 1; m <= max_deg; ++m)
            tmp[m] = coeff[m] + c * tmp[m - 1];

        for (size_t m = 0; m <= max_deg; ++m)
            coeff[m] = tmp[m];

        /* Add the contributions of this level to the final sequence
           a[n] += coeff[n-k]  for n = k, k+1, … , N.
         */
        for (size_t m = 0; m <= max_deg; ++m)
            a_arr[k + m] += coeff[m];
    }

    seq_t result = a_arr[N];

    free(a_arr);
    free(coeff);
    free(tmp);

    return result;
}
