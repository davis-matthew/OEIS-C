/*
 * A237651  G.f. satisfies: A(x) = (1+x+x^2) * A(x^2)^2.
 * https://oeis.org/A237651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 39, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237651: a(0)=1, recurrence from generating function
   A(x) = (1+x+x^2)*A(x^2)^2
   Equivalent linear recurrence in terms of self–convolution:
     f(m) = Σ_{i=0}^{m} a_i*a_{m-i}
     a_{2m+1} = f(m)
     a_{2m}   = f(m) + f(m-1)   (f(-1)=0)
   This implementation builds the sequence up to n in O(n²/8) time. */

#include <stdlib.h>  /* malloc, calloc */

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0) return 1;

    /* size of array needed */
    size_t N = (size_t)n + 1;
    seq_t *a = (seq_t *)calloc(N, sizeof(seq_t));
    if (a == NULL) return 0;  /* out of memory; caller may check */

    /* initial value */
    a[0] = 1;

    /* compute convolution values on the fly, keeping only f(m) and f(m-1) */
    seq_t f_prev = 0;  /* f(-1)=0 */
    seq_t f_curr = 0;
    size_t half = (size_t)n >> 1;  /* floor(n/2) */

    for (size_t m = 0; m <= half; ++m)
    {
        /* compute f(m) = Σ_{i=0}^{m} a_i * a_{m-i} */
        f_curr = 0;
        for (size_t i = 0; i <= m; ++i)
            f_curr += a[i] * a[m - i];

        /* a[2m+1] = f(m) */
        size_t idx1 = (m << 1) + 1;
        if (idx1 <= n)
            a[idx1] = f_curr;

        /* a[2m] = f(m) + f(m-1) */
        size_t idx0 = m << 1;
        if (idx0 <= n)
            a[idx0] = f_curr + f_prev;

        f_prev = f_curr;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
