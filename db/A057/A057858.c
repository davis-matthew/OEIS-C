/*
 * A057858  Number of residue classes modulo n-th primorial number which
 *          contain only composite numbers.
 * https://oeis.org/A057858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} p_i – ∏_{i=1..n} (p_i – 1) – n where p_i is
 *            the i‑th prime (p_1 = 2). For n = 0 or 1 the formula yields 0
 *            (a(0) = a(1) = 0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 0 || n == 1)
        return (seq_t)0;

    /* An upper bound for the n‑th prime.
       For n ≤ 5 the 5th prime is 11, so 15 is enough.
       For larger n a simple linear over‑estimate 20·n + 200
       guarantees enough size (the 10000th prime is 104729). */
    idx_t limit = (n > 5 ? n * 20 + 200 : 15);

    /* Allocate a boolean array for the sieve. */
    char *is_prime = (char *)malloc((size_t)limit + 1);
    if (!is_prime)
        return (seq_t)0;                /* allocation failure – safe fallback */

    /* Initialise all entries to true (1). */
    memset(is_prime, 1, (size_t)limit + 1);
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = 0;
        }
    }

    /* Collect the first n primes. */
    uint32_t *primes = (uint32_t *)malloc((size_t)n * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }

    size_t prime_count = 0;
    for (idx_t i = 2; i <= limit && prime_count < (size_t)n; ++i) {
        if (is_prime[(size_t)i]) {
            primes[prime_count++] = (uint32_t)i;
        }
    }

    /* Compute ∏ p_i and ∏ (p_i – 1). */
    seq_t primorial = (seq_t)1;
    seq_t phi_prod   = (seq_t)1;
    for (size_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)primes[i];
        primorial = primorial * p;
        phi_prod   = phi_prod * ((seq_t)primes[i] - 1);
    }

    /* Clean up dynamic allocations */
    free(primes);
    free(is_prime);

    /* The desired value. */
    return primorial - phi_prod - (seq_t)n;
}
