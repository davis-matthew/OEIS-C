/*
 * A084140  a(n) is the smallest number j such that if x >= j there are at
 *          least n primes between x and 2x exclusively.
 * https://oeis.org/A084140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A084140:  a(n) = smallest j such that for every m ≥ j,
   there are at least n primes strictly between m and 2m.
   The value is found by sieving up to a suitable upper bound and
   checking the prime‑count difference for each m. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

extern seq_t compute_pi(idx_t limit);   /* may be unused */

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* only defined for n ≥ 1 */

    /* A generous upper bound that guarantees the theorem
       π(2m) – π(m) > m/(3 log m) holds for all m ≥ 5. */
    const idx_t U = (n * (idx_t)200 > (idx_t)5) ? n * (idx_t)200 : (idx_t)5;

    /* We need primes up to 2·U – 1 (the rightmost endpoint
       used in the exclusive interval (m,2m)). */
    const idx_t limit = 2 * U;              /* limit ≥ 2·U – 1  */

    /* Allocate sieve array. */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) abort();

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Eratosthenes sieve. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Prefix–sum array of prime counts π(x).  */
    uint32_t *pi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!pi) abort();

    pi[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi[i] = pi[i - 1] + (uint32_t)is_prime[i];

    /* Find the last m with < n primes in (m,2m). */
    idx_t last_bad = 0;
    for (idx_t m = 1; m <= U; ++m) {
        /* Count primes p with m < p < 2m.  */
        uint32_t diff = pi[2 * m - 1] - pi[m];
        if (diff < n) last_bad = m;
    }

    free(is_prime);
    free(pi);

    /* a(n) = last_bad + 1  (smallest m such that all larger m satisfy the property). */
    return (seq_t)(last_bad + 1);
}
