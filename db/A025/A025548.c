/*
 * A025548  a(n) = sum of the exponents in the prime factorization of the
 *          least common multiple of {1,3,5,...,2n-1}.
 * https://oeis.org/A025548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p odd prime ≤ 2n-1} ⌊log_p(2n-1)⌋ * This is the sum
 *            of exponents in the prime factorisation of the * least common
 *            multiple of the odd numbers 1,3,5,…,2n-1
 * Range:     u8 n <= 639, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple sieve to generate all primes ≤ 2n‑1
 * and then for each prime we count the largest power of that prime
 * that does not exceed 2n‑1. The sum of these exponents is returned
 * as a seq_t value. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n to seq_t before arithmetic to avoid overflow on high
     *  n values (even though n is small for the test limits). */
    const seq_t two_n_minus_one = (seq_t)n * 2 - 1;

    /* Handle trivial case directly. */
    if (two_n_minus_one < 3)   /* Only possible for n = 1, 2 */
        return 0;

    /* Allocate a boolean array for the sieve. We use unsigned char
     * because it is the smallest allocatable type that can be indexed
     * by size_t. */
    const size_t lim = (size_t)two_n_minus_one;
    unsigned char *is_prime = (unsigned char *)malloc((lim + 1) * sizeof(unsigned char));
    if (!is_prime)
        return 0;  /* Out‑of‑memory; return 0 as fallback. */

    /* Initialise all entries as prime candidates. */
    for (size_t i = 0; i <= lim; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; (seq_t)i * (seq_t)i <= two_n_minus_one; ++i) {
        if (!is_prime[(size_t)i])
            continue;
        for (size_t j = (size_t)i * (size_t)i; j <= lim; j += (size_t)i)
            is_prime[j] = 0;
    }

    /* Compute the sum of exponents. */

    seq_t total_exponent = 0;

    for (idx_t p = 3; p <= two_n_minus_one; p += 2) {   /* skip even 'p'=2 */
        if (!is_prime[(size_t)p])
            continue;

        seq_t e = 0;
        seq_t pow = 1;
        /* Find the largest exponent e with p^e <= two_n_minus_one. */
        while (pow <= two_n_minus_one / (seq_t)p) {
            pow *= (seq_t)p;
            ++e;
        }

        /* For p > two_n_minus_one, the loop would not run and e stays 0.
         * That cannot happen because the loop only runs for primes <= limit
         * where pow=1 and one multiplication is allowed. */
        total_exponent += e;
    }

    free(is_prime);
    return total_exponent;
}
