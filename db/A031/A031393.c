/*
 * A031393  a(n) = prime(6*n - 4).
 * https://oeis.org/A031393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(6*n-4)
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the n-th term of the sequence: the (6*n-4)-th prime. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset below 1 is undefined; return 0 */

    /* k = index of the desired prime in the prime list (1‑based) */
    idx_t k = (idx_t)6 * n - 4;
    if (k == 0)
        return 0;

    /* Static storage for the prime list, built on demand */
    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_count = 0;
    static uint64_t last_checked = 1;  /* last number tested for primality */

    /* Allocate the prime array if this is the first call */
    if (primes_cap == 0) {
        primes_cap = 256;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    }

    /* Generate primes until we have at least k of them */
    while (primes_count < k) {
        /* Find next candidate */
        ++last_checked;
        if (last_checked > 2 && (last_checked & 1) == 0)
            continue;  /* skip even numbers > 2 */

        bool is_prime = true;
        /* Test divisibility by known primes; stop when p*p > candidate */
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p > last_checked / p)
                break;  /* p*p would overflow or be > candidate */
            if (last_checked % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Expand storage if needed */
            if (primes_count >= primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_count++] = last_checked;
        }
    }

    /* Return the required prime, cast to the build’s seq_t type */
    return (seq_t)primes[k - 1];
}
