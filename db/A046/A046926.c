/*
 * A046926  Number of ways to express n as p+2q; p, q primes.
 * https://oeis.org/A046926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (p,q) of primes such that p + 2q = n
 * Range:     u8 n <= 6824, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple sieve to precompute primes up to the
   current maximum n seen and enumerates all prime q ≤ n/2. */

#include <stdint.h>
#include <stdlib.h>

/* Static data for sieve and prime list */
static idx_t sieve_limit = 0;
static uint8_t *sieve = NULL;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* Build or extend the sieve to cover at least 'limit'.  */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* free previous structures */
    if (sieve)
        free(sieve);
    if (primes)
        free(primes);

    /* allocate and initialize sieve */
    sieve = (uint8_t *)malloc((size_t)(limit + 1));
    for (idx_t i = 0; i <= limit; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Build list of primes up to limit */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            primes[k++] = i;

    prime_cnt   = cnt;
    sieve_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* offset handling, not used */

    ensure_sieve(n);

    seq_t cnt = 0;
    idx_t q_max = n / 2;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = primes[i];
        if (q > q_max)
            break;

        idx_t p = n - 2 * q;
        if (p >= 2 && sieve[p])
            ++cnt;
    }

    return cnt;
}
