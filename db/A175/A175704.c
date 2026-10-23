/*
 * A175704  Auto-convolution of A008472.
 * https://oeis.org/A175704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A008472(k) * A008472(n+1-k) where A008472(k)
 *            is the sum of the distinct primes dividing k
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Ensure n >= 1 */
    if (n < 1) return (seq_t)0;
    /* Allocate array for A008472 values, 1-indexed */
    seq_t *f = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!f) return (seq_t)0;  /* In practice, the system will handle allocation failures. */
    /* Initialize to 0 */
    for (idx_t i = 1; i <= n; ++i) f[i] = 0;
    /* Very small primality sieve */
    unsigned char *is_prime = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    if (!is_prime) { free(f); return (seq_t)0; }
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= n; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= n; j += p)
                is_prime[j] = 0;
    /* Add each prime to its multiples */
    for (idx_t p = 2; p <= n; ++p)
        if (is_prime[p])
            for (idx_t j = p; j <= n; j += p)
                f[j] += (seq_t)p;
    /* Compute the convolution sum */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += f[k] * f[n + 1 - k];
    /* Clean up */
    free(f);
    free(is_prime);
    return sum;
}
