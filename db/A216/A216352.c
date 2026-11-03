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
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: compute the nth Franel number F(n) = sum_{k=0..n} C(n,k)^3 */
static seq_t franel(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;            /* C(n,0) */
    idx_t k;
    for (k = 0; k <= n; ++k) {
        /* bin^3 */
        seq_t power = bin * bin * bin;
        sum += power;
        if (k < n) {
            /* bin = C(n, k+1) = bin * (n-k) / (k+1) */
            bin = bin * (seq_t)(n - k);
            bin = bin / (seq_t)(k + 1);
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays for a[0..n] and B[1..n] */
    seq_t *a_vals = mem_new(seq_t, (n + 1));
    seq_t *B_vals  = mem_new(seq_t, (n + 1));
    if (!a_vals || !B_vals) {
        /* malloc failure; return zero as sentinel */
        if (a_vals) mem_free(a_vals);
        if (B_vals)  mem_free(B_vals);
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
    mem_free(a_vals);
    mem_free(B_vals);
    return result;
}
