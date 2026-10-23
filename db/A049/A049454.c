/*
 * A049454  a(n) = 1 + Sum_{i=1..n} phi(i)^2.
 * https://oeis.org/A049454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{i=1..n} phi(i)^2
 * Range:     u8 n <= 12, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* allocate space for phi[0..n] */
    uint64_t *phi = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!phi) return (seq_t)0;   /* fallback: on allocation failure */

    /* initialise phi[i] = i */
    for (idx_t i = 0; i <= n; ++i) phi[i] = i;

    /* sieve to compute Euler's totient for every i */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {                 /* p is prime */
            for (idx_t j = p; j <= n; j += p) {
                phi[j] -= phi[j] / p;     /* phi[j] = phi[j] * (1 - 1/p)   */
            }
        }
    }

    /* accumulate the sum of squared phi values */
    seq_t sum = (seq_t)1;                  /* the +1 from the definition */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t v = (seq_t)phi[i];
        sum += v * v;
    }

    free(phi);
    return sum;
}
