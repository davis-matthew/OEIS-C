/*
 * A209982  Number of 2 X 2 matrices having all elements in {-n,...,n} and
 *          determinant 1.
 * https://oeis.org/A209982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -12 + 32 * Sum_{k=1..n} phi(k) (for n>0, a(0)=0)
 * Range:     u8 n <= 4, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate an array for phi[1..n] */
    size_t size = (size_t)n + 1;
    uint32_t *phi = (uint32_t *)malloc(size * sizeof(uint32_t));
    if (!phi)
        return 0; /* Fallback in case of allocation failure */

    /* Initialize phi[i] = i */
    for (size_t i = 1; i < size; ++i)
        phi[i] = (uint32_t)i;

    /* Euler's sieve to compute phi values */
    for (size_t p = 2; p < size; ++p) {
        if (phi[p] == (uint32_t)p) {            /* p is prime */
            for (size_t j = p; j < size; j += p)
                phi[j] -= phi[j] / (uint32_t)p;
        }
    }

    /* Compute the prefix sum of phi up to n */
    seq_t sum_phi = 0;
    for (size_t i = 1; i < size; ++i)
        sum_phi += (seq_t)phi[i];

    free(phi);

    /* a(n) = 32 * sum_phi - 12 */
    return (seq_t)32 * sum_phi - (seq_t)12;
}
