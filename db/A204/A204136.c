/*
 * A204136  Number of composites between successive twin prime pairs.
 * https://oeis.org/A204136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of composites between the n‑th and (n+1)‑st twin
 *            prime pairs (open interval (p+2, q), where (p,p+2) and (q,q+2)
 *            are successive twin prime pairs)
 * Range:     u8 n <= 324, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static idx_t sieve_limit = 0;
    static uint64_t *primes    = NULL;          /* list of primes up to sieve_limit */
    static uint32_t *prime_pref = NULL;         /* cumulative prime count up to i */
    static uint64_t *twin_list = NULL;          /* lower prime of each twin pair */
    static size_t prime_cnt  = 0;
    static size_t twin_cnt   = 0;

    /* We need enough twin pairs to answer the query: indices 0..n */
    while (twin_cnt <= (size_t)n) {
        idx_t new_limit = (sieve_limit == 0) ? 10000 : sieve_limit * 2;
        if (new_limit < 30) new_limit = 30;  /* small start */

        /* sieve of Eratosthenes */
        unsigned char *is_prime = (unsigned char *)malloc((size_t)new_limit + 1);
        if (!is_prime) exit(1);
        for (size_t i = 0; i <= (size_t)new_limit; ++i) is_prime[i] = 1;
        is_prime[0] = 0;
        is_prime[1] = 0;
        for (idx_t i = 2; (i * i) <= new_limit; ++i) {
            if (is_prime[(size_t)i]) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    is_prime[(size_t)j] = 0;
            }
        }

        /* count primes and build arrays */
        size_t new_prime_cnt = 0;
        for (idx_t i = 2; i <= new_limit; ++i)
            if (is_prime[(size_t)i]) ++new_prime_cnt;

        uint64_t *new_primes =
            (uint64_t *)malloc(new_prime_cnt * sizeof(uint64_t));
        uint32_t *new_prefix =
            (uint32_t *)malloc(((size_t)new_limit + 1) * sizeof(uint32_t));

        size_t idx = 0;
        uint32_t sum = 0;
        new_prefix[0] = 0;
        for (idx_t i = 1; i <= new_limit; ++i) {
            if (is_prime[(size_t)i]) {
                new_primes[idx++] = (uint64_t)i;
                ++sum;
            }
            new_prefix[(size_t)i] = sum;
        }

        /* build twin list */
        size_t new_twin_cnt = 0;
        for (size_t i = 0; i + 1 < new_prime_cnt; ++i)
            if (new_primes[i + 1] == new_primes[i] + 2)
                ++new_twin_cnt;

        uint64_t *new_twin =
            (uint64_t *)malloc(new_twin_cnt * sizeof(uint64_t));
        new_twin_cnt = 0;
        for (size_t i = 0; i + 1 < new_prime_cnt; ++i)
            if (new_primes[i + 1] == new_primes[i] + 2)
                new_twin[new_twin_cnt++] = new_primes[i];

        /* free old data */
        free(primes);
        free(prime_pref);
        free(twin_list);
        free(is_prime);

        primes    = new_primes;
        prime_pref = new_prefix;
        twin_list = new_twin;
        prime_cnt  = new_prime_cnt;
        twin_cnt   = new_twin_cnt;
        sieve_limit = new_limit;
    }

    /* retrieve the two successive twin pairs */
    uint64_t p = twin_list[n - 1];   /* lower prime of the n-th pair   */
    uint64_t q = twin_list[n];      /* lower prime of the (n+1)-st pair */

    /* if the pairs are adjacent or overlapping, no composites in between */
    if (q <= p + 2) return (seq_t)0;

    /* number of integers in the open interval (p+2, q) */
    seq_t interval = (seq_t)(q - p - 3);

    /* number of primes in that open interval */
    uint32_t primes_le_q_minus1 = prime_pref[(size_t)(q - 1)];
    uint32_t primes_le_p_plus2  = prime_pref[(size_t)(p + 2)];
    seq_t primes_in_interval =
        (seq_t)(primes_le_q_minus1 - primes_le_p_plus2);

    /* composites = all integers - primes */
    return interval - primes_in_interval;
}
