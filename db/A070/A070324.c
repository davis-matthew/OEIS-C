/*
 * A070324  a(n) = Max( sigma(k) : k=1,2,3,...,n ).
 * https://oeis.org/A070324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max{σ(k) : 1 ≤ k ≤ n}
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Treat idx_t as an unsigned integer type */
    size_t N = (size_t)n;
    if (N < 1) return (seq_t)0;          /* safety for n = 0, though offset is 1 */

    /* Allocate an array to hold divisor sums σ(0)…σ(N) */
    seq_t *sigma = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!sigma)
        abort();                         /* allocation failure */

    /* Initialise to zero; works for built‑in types and for user types
       with a default constructor and assignment from 0. */
    for (size_t i = 0; i <= N; ++i)
        sigma[i] = (seq_t)0;

    /* Sieve: for each divisor d add d to all multiples d, 2d, 3d, … */
    for (size_t d = 1; d <= N; ++d) {
        seq_t add = (seq_t)d;
        for (size_t m = d; m <= N; m += d)
            sigma[m] += add;
    }

    /* Scan for the maximum divisor sum up to N. */
    seq_t max_sigma = (seq_t)0;
    for (size_t k = 1; k <= N; ++k) {
        if (sigma[k] > max_sigma)
            max_sigma = sigma[k];
    }

    free(sigma);
    return max_sigma;
}
