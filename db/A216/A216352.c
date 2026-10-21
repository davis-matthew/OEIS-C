/*
 * A216352  G.f.: A(x) = exp( Sum_{n>=1} A000172(n)^2*x^n/n ) where Franel
 *          number A000172(n) = Sum_{k=0..n} C(n,k)^3.
 * https://oeis.org/A216352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp( Sum_{m>=1} A000172(m)^2 * x^m / m ); a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   franel
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/franel.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays for a[0..n] and B[1..n] */
    seq_t *a_vals = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *B_vals  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a_vals || !B_vals) {
        /* malloc failure; return zero as sentinel */
        if (a_vals) free(a_vals);
        if (B_vals)  free(B_vals);
        return (seq_t)0;
    }

    a_vals[0] = (seq_t)1;   /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute B[i] = Franel(i)^2 */
        seq_t f = franel(i);
        B_vals[i] = f * f;

        /* a[i] = (1/i) * sum_{k=1..i} a[i-k] * B[k] */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += a_vals[i - k] * B_vals[k];
        }
        a_vals[i] = sum / (seq_t)i;
    }

    seq_t result = a_vals[n];
    free(a_vals);
    free(B_vals);
    return result;
}
