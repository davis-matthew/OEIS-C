/*
 * A067434  Number of distinct prime factors in binomial(2*n,n).
 * https://oeis.org/A067434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct prime factors of binomial(2*n,n)
 * Range:     u8 n <= 1283, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple sieve with static prime cache. */
#include <stdint.h>
#include <stdlib.h>

/* static storage for primes */
static uint64_t *primes = NULL;   /* array of primes */
static size_t   prime_cnt = 0;     /* number of stored primes */
static idx_t    prime_limit = 0;   /* max value for which primes[] contains all primes */

/* Ensure that primes[] contains every prime ≤ need_limit */
static void
ensure_primes_upto(idx_t need_limit)
{
    if (need_limit <= prime_limit)
        return;   /* already up to date */

    size_t limit = (size_t)need_limit;

    /* Allocate sieve and prime list */
    uint8_t *sieve = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!sieve)
        return; /* allocation failed – keep old primes */

    size_t max_primes = limit + 10;      /* generous upper bound */
    uint64_t *new_primes = (uint64_t *)malloc(max_primes * sizeof(uint64_t));
    if (!new_primes) {
        free(sieve);
        return;
    }

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {                    /* i is prime */
            if (count < max_primes)
                new_primes[count++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }
    }

    /* Replace old prime list */
    free(primes);
    primes     = new_primes;
    prime_cnt  = count;
    prime_limit = need_limit;
    free(sieve);
}

/* a(n) = number of distinct prime factors of binomial(2*n,n) */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset, but safe */

    idx_t two_n = n * 2;
    ensure_primes_upto(two_n);

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > (uint64_t)two_n)
            break;

        /* Legendre's formula: exponent of p in (2n choose n) */
        idx_t exp = 0;
        idx_t power = p;
        while (power <= two_n) {
            exp += (two_n / power) - 2 * (n / power);
            if (power > two_n / p)
                break;
            power *= p;
        }

        if (exp > 0)
            ++count;
    }

    return count;
}
