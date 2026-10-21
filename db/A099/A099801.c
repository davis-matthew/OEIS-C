/*
 * A099801  PrimePi(2n+1), the number of primes less than or equal to 2n+1.
 * https://oeis.org/A099801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = PrimePi(2*n+1), the number of primes ≤ 2*n+1
 * Range:     u8 n <= 808, u16 n <= 9814, u32 n <= 9814, u64 n <= 9814,
 *            u128 n <= 9814, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n = 0 gives target 1, which has no primes */
    idx_t target = (idx_t)2 * n + 1;

    /* Allocate a byte array for the sieve; size = target+1 */
    char *is_prime = (char *)malloc((size_t)(target + 1));
    if (!is_prime) return 0; /* unlikely, but guard against OOM */

    /* Initialize all entries as prime (1), except 0 and 1 */
    for (size_t i = 0; i <= (size_t)target; ++i) is_prime[i] = 1;
    if (target >= 0) is_prime[0] = 0;
    if (target >= 1) is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p <= target / p; ++p)
    {
        if (is_prime[p])
        {
            for (idx_t mult = p * p; mult <= target; mult += p)
                is_prime[mult] = 0;
        }
    }

    /* Count primes up to target */
    seq_t count = 0;
    for (idx_t i = 2; i <= target; ++i)
        if (is_prime[i]) ++count;

    free(is_prime);
    return count;
}
