/*
 * A208588  Row square-sums of triangle A185384.
 * https://oeis.org/A208588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum(binomial(n,k)^2 * F(2n-k+1)^2, k=0..n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate Fibonacci array up to index 2*n+1 */
    idx_t size = 2 * n + 2;                  /* indices [0 .. 2n+1] */
    seq_t *fib = (seq_t *)malloc((size_t)size * sizeof(seq_t));
    if (!fib) return 0;                      /* allocation failure */

    fib[0] = 0;
    fib[1] = 1;
    for (idx_t i = 2; i < size; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    seq_t sum  = 0;
    seq_t bin  = 1;                          /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* binom(n,k) = binom(n,k-1) * (n-k+1) / k  */
            bin = bin * (seq_t)(n - k + 1) / (seq_t)k;
        }

        idx_t idx = 2 * n - k + 1;           /* index for F(2n-k+1) */
        seq_t f   = fib[idx];

        /* term = bin^2 * f^2 */
        seq_t term = bin * bin;
        term = term * f;
        term = term * f;

        sum += term;
    }

    free(fib);
    return sum;
}
