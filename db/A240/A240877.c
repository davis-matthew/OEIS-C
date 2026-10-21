/*
 * A240877  Sum of the denominators of the Farey series of order n (A006843).
 * https://oeis.org/A240877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} k * phi(k) (phi = Euler totient)
 * Range:     u8 n <= 10, u16 n <= 68, u32 n <= 2766, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate array for Euler's totient values up to n */
    idx_t *phi = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!phi) return (seq_t)1;          /* fallback if allocation fails */

    /* Initialisation: phi[i] = i */
    for (idx_t i = 0; i <= n; ++i) phi[i] = i;

    /* Sieve to compute phi */
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == i) {                 /* i is prime */
            for (idx_t j = i; j <= n; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }

    /* Accumulate the sum in arbitrary‑precision type seq_t */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = (seq_t)k * (seq_t)phi[k];
        sum += term;
    }

    free(phi);
    return (seq_t)1 + sum;
}
