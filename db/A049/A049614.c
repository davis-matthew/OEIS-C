/*
 * A049614  n! divided by its squarefree kernel.
 * https://oeis.org/A049614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of composite numbers ≤ n = n! divided by the
 *            primorial of n (product of primes ≤ n)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n = 0,1 give 1 */
    if (n < 2)
        return (seq_t)1;

    /* Sieve of Eratosthenes up to n */
    char *is_prime = (char *)malloc((size_t)(n + 1) * sizeof(char));
    if (!is_prime)
        return (seq_t)0; /* out‑of‑memory – not expected for the limits */

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = 0;
        }
    }

    /* Compute product of composites */
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_prime[i])
            result *= (seq_t)i;
    }

    free(is_prime);
    return result;
}
