/*
 * A097454  a(n) = (number of nonprimes <= n) - (number of primes <= n).
 * https://oeis.org/A097454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - 2*π(n) (where π(n) is the number of primes ≤ n)
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1, return 0 */

    /* Allocate a boolean array to mark primes up to n.  The cast is needed
       for C++ compatibility. */
    bool *is_prime = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    if (!is_prime)
        return 0;                     /* memory allocation failed */

    /* Initialise all entries as prime. Then clear 0 and 1. */
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    /* Eratosthenes sieve.  Use n / p to avoid overflow of p * p. */
    for (idx_t p = 2; p <= n / p; ++p) {
        if (is_prime[p]) {
            for (idx_t mult = p * p; mult <= n; mult += p)
                is_prime[mult] = false;
        }
    }

    /* Count primes ≤ n. */
    idx_t primes = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            ++primes;

    free(is_prime);

    /* Compute a(n) = n - 2 * π(n). */
    return (seq_t)n - ((seq_t)2 * (seq_t)primes);
}
